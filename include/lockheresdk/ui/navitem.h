#ifndef LOCKHERE_UI_UTILS_NAVITEM_H
#define LOCKHERE_UI_UTILS_NAVITEM_H

#include <QPushButton>
#include <QLabel>

namespace LockHere {
namespace Ui {
namespace utils {

struct NavItemPrivate;

class NavItem : public QWidget {
	Q_OBJECT

	Q_PROPERTY(QString state MEMBER state_)

public:
	explicit NavItem(QWidget* parent = nullptr);
	NavItem(const QString& text, const QPixmap& icon, const QString& hint,
			QWidget* parent = nullptr);
	NavItem(const QString& text, const QString& svgPath, const QString& hint,
			QWidget* parent = nullptr);
	NavItem(const QString& text, const QPixmap& icon, QWidget* parent = nullptr);
	NavItem(const QString& text, const QString& svgPath, QWidget* parent = nullptr);
	~NavItem();

	QSize sizeHint() const Q_DECL_OVERRIDE;
	void  paintEvent(QPaintEvent* event) Q_DECL_OVERRIDE;
	void  enterEvent(QEnterEvent* event) Q_DECL_OVERRIDE;
	void  leaveEvent(QEvent* event) Q_DECL_OVERRIDE;
	void  mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
	void  mouseReleaseEvent(QMouseEvent* event) Q_DECL_OVERRIDE;

	QString navId() const;

	QString state() const;
	void	updateState(bool shouldRepaint = true);

	QString text() const;
	void	setText(const QString& text);

	QString hint() const;
	void	setHint(const QString& text);

	bool isExpandable() const;
	void setExpandable(bool expandable);

	void setExpanded(bool expanded);
	bool isExpanded() const;

	void setSelected(bool selected);
	bool isSelected() const;

	void setIcon(const QPixmap& icon);
	void setIcon(const QString& svgPath);

signals:
	void clicked();
	void selected();

private:
	void emit_clicked();

private:
	NavItemPrivate* d;

	const int	  spacing_ = 4;
	const QString navId_;
	QString		  state_;

	QString text_;
	QString hint_;
	QString svgIconPath_;
	QPixmap icon_;
	bool	expandable_;
	bool	expanded_;
	bool	selected_;
};

}	// namespace utils
}	// namespace Ui
}	// namespace LockHere

#endif	 // LOCKHERE_UI_UTILS_NAVITEM_H
