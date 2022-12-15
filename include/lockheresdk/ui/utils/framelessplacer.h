#ifndef FRAMELESSPLACER_H
#define FRAMELESSPLACER_H

#include <QWidget>

namespace LockHere {

class FramelessPlacer : public QWidget {
	Q_OBJECT
public:
	explicit FramelessPlacer(QWidget* parent = nullptr);
	~FramelessPlacer();

	//! 重置内容控件
    void	 setBackgroundColor(const QColor& bgColor);
    void	 setBorderRadius(int borderRadius);
    void	 installStyleSheet(const QString& url);
    QWidget* setContent(QWidget* content);

	//! 拖拽支持
	void mousePressEvent(QMouseEvent* e) Q_DECL_OVERRIDE;
	void mouseReleaseEvent(QMouseEvent* e) Q_DECL_OVERRIDE;
	void mouseMoveEvent(QMouseEvent* e) Q_DECL_OVERRIDE;

signals:
    void shouldMinimizeToTray();
    void shouldMinimize();
    void shouldClose();

public:
	static constexpr int shadowRadius = 4;	 //!< 阴影半径

private:
	struct FramelessPlacerPrivate;
	FramelessPlacerPrivate* data_;

signals:
};

}	// namespace LockHere

#endif	 // FRAMELESSPLACER_H
