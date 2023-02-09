#include <lockheresdk/ui/navitem.h>

#include <lockheresdk/ui/utils/svghelper.h>
#include <QLayout>
#include <QUuid>
#include <QStylePainter>
#include <QStyle>
#include <QMouseEvent>
#include <QStyleOption>
#include <QPaintEvent>
#include <QPainter>
#include <QFile>
#include <QTimer>
#include <QApplication>

namespace LockHere {
namespace Ui {
namespace utils {

struct NavItemPrivate {
	NavItemPrivate()
		: down(false)
		, pressed(false)
		, hover(false) {}
	bool down;
	bool pressed;
	bool hover;
};

NavItem::NavItem(QWidget* parent)
	: QWidget{parent}
	, navId_{QUuid::createUuid().toString()}
	, d{new NavItemPrivate} {
	setFocusPolicy(Qt::ClickFocus);
	setAttribute(Qt::WA_Hover, true);

	expandable_ = false;
	expanded_	= false;
	selected_	= false;
	updateState(false);
}

NavItem::NavItem(const QString& text, const QPixmap& icon, const QString& hint, QWidget* parent)
	: NavItem(parent) {
	setText(text);
	setIcon(icon);
	setHint(hint);
}

NavItem::NavItem(const QString& text, const QString& svgPath, const QString& hint, QWidget* parent)
	: NavItem(parent) {
	setText(text);
	setIcon(svgPath);
	setHint(hint);
}

NavItem::NavItem(const QString& text, const QPixmap& icon, QWidget* parent)
	: NavItem(parent) {
	setText(text);
	setIcon(icon);
	setExpandable(true);
	setExpanded(false);
}

NavItem::NavItem(const QString& text, const QString& svgPath, QWidget* parent)
	: NavItem(parent) {
	setText(text);
	setIcon(svgPath);
	setExpandable(true);
	setExpanded(false);
}

NavItem::~NavItem() {
	delete d;
}

QSize NavItem::sizeHint() const {
	return QSize(0, fontMetrics().height() + spacing_ * 2);
}

void NavItem::paintEvent(QPaintEvent* event) {
	Q_UNUSED(event);

	if (underMouse() != d->hover) {
		d->hover = underMouse();
		updateState(false);
	}

	QPainter painter(this);

	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.setRenderHint(QPainter::TextAntialiasing, true);

	QStyleOption opt;
	opt.initFrom(this);

	style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);

	auto r = rect();
	r.adjust(spacing_, spacing_, -spacing_, -spacing_);

	auto width	  = opt.fontMetrics.height();
	auto iconSize = QSize(width, width) * 0.8;
	auto yOffset  = (r.height() - iconSize.height()) / 2;

	QRect iconRect(r.left() + yOffset, r.top() + yOffset, iconSize.width(), iconSize.height());
	QRect ExpandRect = iconRect.translated(r.width() - iconRect.right() - yOffset, 0);

	if (!svgIconPath_.isNull()) {
		auto pixmap = loadSvgAsPixmap(svgIconPath_, iconSize * 4, painter.pen().color());
		painter.drawPixmap(iconRect, *pixmap);
		//! NOTE: see comment in src/sysmenubar.cpp
		// delete pixmap;
	} else if (!icon_.isNull()) {
		painter.drawPixmap(iconRect, icon_);
	}

	QRect bb;
	auto  textRect = r.adjusted(yOffset * 2 + iconRect.width(), 0, 0, 0);
	painter.drawText(textRect, Qt::AlignVCenter, text_, &bb);

	if (!isExpandable()) {
		textRect.adjust(bb.width() + spacing_, 0, 0, 0);
		painter.drawText(textRect, Qt::AlignVCenter | Qt::AlignRight, hint_);
	} else {
		QString expandIconPath =
			isExpanded() ? ":/icons/chevron-down.svg" : ":/icons/chevron-right.svg";
		auto pixmap = loadSvgAsPixmap(expandIconPath, iconSize * 4, painter.pen().color());
		painter.drawPixmap(ExpandRect, *pixmap);
		//! NOTE: see comment above
		// delete pixmap;
	}
}

void NavItem::enterEvent(QEnterEvent* event) {
	d->hover = true;
	updateState();
}

void NavItem::leaveEvent(QEvent* event) {
	d->hover = false;
	if (d->down) {
		d->down	   = false;
		d->pressed = false;
	}
	updateState();
}

void NavItem::mousePressEvent(QMouseEvent* event) {
	if (event->button() != Qt::LeftButton) {
		event->ignore();
		return;
	}
	if (rect().contains(event->pos())) {
		d->down	   = true;
		d->pressed = true;
		updateState();
		event->accept();
	} else {
		event->ignore();
	}
}

void NavItem::mouseReleaseEvent(QMouseEvent* event) {
	if (event->button() != Qt::LeftButton || !d->down) {
		event->ignore();
		return;
	}
	if (rect().contains(event->pos())) {
		d->down	   = false;
		d->pressed = false;
		emit_clicked();
		updateState();
		event->accept();
	} else {
		event->ignore();
	}
}

QString NavItem::navId() const {
	return navId_;
}

QString NavItem::state() const {
	return state_;
}

void NavItem::updateState(bool shouldRepaint) {
	QString newState;
	if (d->pressed) {
		newState = "pressed";
	} else if (isSelected() && !isExpandable()) {
		newState = "selected";
	} else if (d->hover) {
		newState = "hover";
	} else {
		newState = "default";
	}
	if (newState != state_) {
		state_ = newState;
		style()->unpolish(this);
		style()->polish(this);
		if (shouldRepaint) {
			repaint();
		}
	}
}

QString NavItem::text() const {
	return text_;
}

void NavItem::setText(const QString& text) {
	text_ = text;
}

QString NavItem::hint() const {
	return hint_;
}

void NavItem::setHint(const QString& hint) {
	hint_ = hint;
}

bool NavItem::isExpandable() const {
	return expandable_;
}

void NavItem::setExpandable(bool expandable) {
	expandable_ = expandable;
}

void NavItem::setExpanded(bool expanded) {
	if (!isExpandable()) return;
	setSelected(expanded);
}

bool NavItem::isExpanded() const {
	return isExpandable() ? expanded_ : false;
}

void NavItem::setSelected(bool selected) {
	selected_ = selected;
	if (isExpandable()) {
		expanded_ = selected;
	}
	if (isSelected()) {
		emit this->selected();
	}
}

bool NavItem::isSelected() const {
	return selected_;
}

void NavItem::setIcon(const QPixmap& icon) {
	icon_ = icon;
	svgIconPath_.clear();
}

void NavItem::setIcon(const QString& svgPath) {
	QFile file(svgPath);
	file.open(QFile::ReadOnly);
	if (!file.isOpen()) return;
	svgIconPath_ = svgPath;
}

void NavItem::emit_clicked() {
	emit clicked();
	setSelected(!isSelected());
	updateState();
}

}	// namespace utils
}	// namespace Ui
}	// namespace LockHere
