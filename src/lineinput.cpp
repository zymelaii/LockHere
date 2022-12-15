#include <lockheresdk/ui/lineinput.h>
#include "lineinput_p.h"

#include <lockheresdk/ui/utils/svghelper.h>
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

QLineEditTrick::QLineEditTrick(QWidget* parent)
    : QLineEdit(parent) {}

void QLineEditTrick::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::MouseButton::LeftButton) {
        if (!hasFocus()) {
            setFocus(Qt::MouseFocusReason);
        }
    }
    QLineEdit::mousePressEvent(event);
}

void QLineEditTrick::focusOutEvent(QFocusEvent* event) {
    QLineEdit::focusOutEvent(event);
    emit loseFocus();
}

LineInput::LineInput(QWidget* parent)
    : QWidget{parent}
    , btOption{nullptr} {

    setMinimumSize(256, 64);
    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    setFocusPolicy(Qt::NoFocus);

    hloMain = new QHBoxLayout(this);
    vloEdit = new QVBoxLayout;
    hloMain->setSpacing(8);
    vloEdit->setSpacing(0);

    lbHint = new QLabel(this);
    {
        QFont font("Open Sans", 12);
        font.setCapitalization(QFont::Capitalize);
        lbHint->setFont(font);
        lbHint->setMargin(4);
    }
    lbHint->setObjectName("hint");
    lbHint->setFocusPolicy(Qt::StrongFocus);
    lbHint->installEventFilter(this);

    leInput = new QLineEditTrick(this);
    {
        QFont font("Open Sans", 16);
        font.setBold(true);
        leInput->setFont(font);
        leInput->setVisible(false);
    }
    leInput->setObjectName("input");
    leInput->setFocusPolicy(Qt::ClickFocus);
    leInput->installEventFilter(this);

    btOption = new QPushButton(this);
    btOption->setFocusPolicy(Qt::NoFocus);
    btOption->setFixedSize(48, 48);
    btOption->setIconSize(btOption->size() / 2);
    btOption->setObjectName("option");
    setOptionEnabled(true);
    btOption->installEventFilter(this);

    vloEdit->addWidget(lbHint);
    hloMain->addLayout(vloEdit);
    hloMain->addWidget(btOption);

    connect(leInput, &QLineEdit::textChanged, this, &LineInput::textChanged);
    connect(btOption, &QPushButton::clicked, this, &LineInput::clicked);
    connect(static_cast<QLineEditTrick*>(leInput),
            &QLineEditTrick::loseFocus,
            this,
            &LineInput::onLoseFocus);
}

bool LineInput::eventFilter(QObject* obj, QEvent* event) {
    if (obj == leInput || obj == lbHint) {
        if (event->type() == QEvent::FocusIn) {
            setInputVisible(true);
            lbHint->setFocusProxy(leInput);
            lbHint->setFocusPolicy(Qt::NoFocus);
            setFocusProxy(leInput);
            focusInEvent(static_cast<QFocusEvent*>(event));
        } else if (event->type() == QEvent::FocusOut) {
            focusOutEvent(static_cast<QFocusEvent*>(event));
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

void LineInput::setHint(const QString& hint) {
    lbHint->setText(hint);
}

void LineInput::setInputVisible(bool visible) {
    if (leInput->isVisible() == visible) return;

    if (visible) {
        QFont font("Open Sans", 8);
        font.setCapitalization(QFont::AllUppercase);
        lbHint->setFont(font);
        lbHint->setMargin(0);

        vloEdit->addWidget(leInput);
    } else {
        QFont font("Open Sans", 12);
        font.setCapitalization(QFont::Capitalize);
        lbHint->setFont(font);
        lbHint->setMargin(4);

        vloEdit->removeWidget(leInput);
    }

    leInput->setVisible(visible);
}

void LineInput::setOptionEnabled(bool enabled) {
    btOption->setEnabled(enabled);
}

void LineInput::setOptionVisible(bool visible) {
    btOption->setVisible(visible);
}

void LineInput::setOptionIcon(const QPixmap& icon) {
    btOption->setIcon(icon);
}

void LineInput::setOptionIcon(const QString& svgPath, QColor currentColor) {
    QPixmap* pSvgPixmap = loadSvgAsPixmap(svgPath, btOption->size(), currentColor);
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

void LineInput::onLoseFocus() {
    lbHint->setFocusProxy(nullptr);
    lbHint->setFocusPolicy(Qt::StrongFocus);
    if (leInput->text().isEmpty()) {
        setInputVisible(false);
    }
    setFocusProxy(nullptr);
}

}	// namespace utils
}	// namespace Ui
}	// namespace LockHere
