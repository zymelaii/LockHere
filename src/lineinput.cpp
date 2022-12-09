#include <lockheresdk/ui/lineinput.h>

#include <lockheresdk/svghelper.h>
#include <QDebug>
#include <QFont>
#include <QFocusEvent>
#include <QStyleOption>
#include <QPainter>
#include <QApplication>
#include <QSizePolicy>

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
        border-radius: 8px;
        border: 1px solid #303030;
    )");

	hloMain = new QHBoxLayout;
	vloEdit = new QVBoxLayout;
	hloMain->setSpacing(8);
	vloEdit->setSpacing(0);

	lbHint = new QLabel(this);
	{
		QFont font("Open Sans", 12);
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
		QFont font("Open Sans", 16);
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
	connect(leInput, &QLineEdit::textChanged, this, &LineInput::textChanged);
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

void LineInput::setOptionEnabled(bool enabled) {
	if (!enabled && !btOption) return;

	if (enabled) {
		btOption = new QPushButton(this);
		btOption->setFocusPolicy(Qt::NoFocus);
		btOption->setFixedSize(48, 48);
		btOption->setObjectName("btOption");
		btOption->setStyleSheet(R"(
		#btOption {
			background: transparent;
			border-radius: 24;
			border: none;
		}
		#btOption:hover {
			background: #303030;
		}
		#btOption:pressed {
			background: #5d5d5d;
		})");
		hloMain->addWidget(btOption);
		btOption->show();
		connect(btOption, &QPushButton::clicked, this, &LineInput::clicked);
	} else {
		btOption->hide();
		hloMain->removeWidget(btOption);
		delete btOption;
		btOption = nullptr;
	}
}

void LineInput::setOptionIcon(const QPixmap& icon) {
	if (!btOption) return;
	btOption->setIconSize(btOption->size() / 2);
	btOption->setIcon(icon);
}

void LineInput::setOptionIcon(const QString& svgPath, QColor currentColor) {
	if (!btOption) return;
	QPixmap* pSvgPixmap = loadSvgAsPixmap(svgPath, btOption->size(), currentColor);
	btOption->setIconSize(btOption->size() / 2);
	btOption->setIcon(*pSvgPixmap);
	delete pSvgPixmap;
}

LineInput::EchoMode LineInput::echoMode() const {
	return leInput->echoMode();
}

void LineInput::setEchoMode(EchoMode mode) {
	leInput->setEchoMode(mode);
}

QString LineInput::text() const {
	return leInput->text();
}

void LineInput::onActivate() {
	if (leInput->isVisible() && leInput->text().isEmpty()) return;

	setStyleSheet(R"(
        background: #252525;
        border-radius: 8px;
        border: 1px solid #3ab0ef;
    )");

	{
		QFont font("Open Sans", 8);
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
        border-radius: 8px;
        border: 1px solid #303030;
    )");

	if (!leInput->text().isEmpty()) {
		emit leInput->editingFinished();
		return;
	}

	{
		QFont font("Open Sans", 12);
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
