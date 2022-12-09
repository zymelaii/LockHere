#include "lineinput.h"

#include <QDebug>
#include <QFont>
#include <QFocusEvent>
#include <QStyleOption>
#include <QPainter>
#include <QApplication>

namespace LockHere {
namespace Ui {
namespace utils {

class QLineEditTrick : public QLineEdit {
public:
	explicit QLineEditTrick(QWidget* parent = nullptr)
		: QLineEdit(parent) {}

	void focusInEvent(QFocusEvent* event) Q_DECL_OVERRIDE { QLineEdit::focusInEvent(event); }

	void focusOutEvent(QFocusEvent* event) Q_DECL_OVERRIDE { QLineEdit::focusOutEvent(event); }
};

LineInput::LineInput(QWidget* parent)
	: QWidget{parent}
	, btOption{nullptr} {

	setMinimumSize(256, 64);
	setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);

	setFocusPolicy(Qt::StrongFocus);

	setStyleSheet(R"(
        background: #252525;
        border-radius: 16px;
        border: 1px solid #303030;
    )");

	hloMain = new QHBoxLayout;
	vloEdit = new QVBoxLayout;
	hloMain->setSpacing(0);
	vloEdit->setSpacing(0);

	lbHint = new QLabel(this);
	{
		QFont font;
		font.setFamily("Open Sans");
		font.setPointSize(12);
		font.setCapitalization(QFont::Capitalize);
		lbHint->setFont(font);
	}
	lbHint->setObjectName("lbHint");
	lbHint->setStyleSheet(R"(#lbHint {
        background: transparent;
        color: #7d7d7d;
        border: none;
    })");
	lbHint->setMargin(4);

	leInput = new QLineEditTrick(this);
	{
		QFont font;
		font.setFamily("Open Sans");
		font.setPointSize(16);
		font.setBold(true);
		leInput->setFont(font);
	}
	leInput->setObjectName("leInput");
	leInput->setStyleSheet(R"(#leInput {
        background: transparent;
        color: #f0f0f0;
        border: none;
    })");
	leInput->setHidden(true);
	leInput->setFocusPolicy(Qt::ClickFocus);
	leInput->installEventFilter(this);

	vloEdit->addWidget(lbHint);
	hloMain->addLayout(vloEdit);
	setLayout(hloMain);

	connect(this, SIGNAL(activate()), this, SLOT(onActivate()));
	connect(this, SIGNAL(deactivate()), this, SLOT(onDeactivate()));

	connect(leInput, SIGNAL(returnPressed()), this, SLOT(onFocusNext()));
}

bool LineInput::eventFilter(QObject* obj, QEvent* event) {
	if (event->type() == QEvent::MouseButtonPress) {
		if (obj != leInput) {
			emit deactivate();
			return true;
		}
	}

	if (event->type() == QEvent::FocusIn) {
		if (obj == leInput && !leInput->text().isEmpty()) {
			auto e = reinterpret_cast<QFocusEvent*>(event);
			static_cast<QLineEditTrick*>(leInput)->focusInEvent(e);
			emit activate();
			return true;
		}
	}

	if (event->type() == QEvent::FocusOut) {
		if (obj == leInput) {
			auto e = reinterpret_cast<QFocusEvent*>(event);
			static_cast<QLineEditTrick*>(leInput)->focusOutEvent(e);
			emit deactivate();
			return true;
		}
	}

	return QWidget::eventFilter(obj, event);
}

void LineInput::paintEvent(QPaintEvent* e) {
	Q_UNUSED(e);

	QStyleOption opt;
	QPainter	 painter(this);

	opt.initFrom(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}

void LineInput::focusInEvent(QFocusEvent* e) {
	emit activate();
}

void LineInput::focusOutEvent(QFocusEvent* e) {
	if (!leInput->hasFocus()) {
		emit deactivate();
	}
}

void LineInput::setHint(const QString& hint) {
	lbHint->setText(hint);
}

void LineInput::onActivate() {
	if (leInput->isVisible() && leInput->text().isEmpty()) return;

	setStyleSheet(R"(
        background: #252525;
        border-radius: 16px;
        border: 1px solid #3ab0ef;
    )");

	{
		QFont font;
		font.setFamily("Open Sans");
		font.setPointSize(8);
		font.setCapitalization(QFont::AllUppercase);
		lbHint->setFont(font);
	}
	lbHint->setMargin(0);
	lbHint->setStyleSheet(R"(#lbHint {
        background: transparent;
        color: #3ab0ef;
        border: none;
    })");

	if (leInput->isHidden()) {
		vloEdit->addWidget(leInput);
		leInput->setHidden(false);
	}

	leInput->activateWindow();
	leInput->setFocus();
}

void LineInput::onDeactivate() {
	if (leInput->isHidden()) return;

	setStyleSheet(R"(
        background: #252525;
        border-radius: 16px;
        border: 1px solid #303030;
    )");

	if (!leInput->text().isEmpty()) {
		emit leInput->editingFinished();
		return;
	}

	{
		QFont font;
		font.setFamily("Open Sans");
		font.setPointSize(12);
		font.setCapitalization(QFont::Capitalize);
		lbHint->setFont(font);
	}
	lbHint->setObjectName("lbHint");
	lbHint->setStyleSheet(R"(#lbHint {
        background: transparent;
        color: #7d7d7d;
        border: none;
    })");
	lbHint->setMargin(4);

	leInput->setHidden(true);
	vloEdit->removeWidget(leInput);
}

void LineInput::onFocusNext() {
	focusNextChild();
}

}	// namespace utils
}	// namespace Ui
}	// namespace LockHere
