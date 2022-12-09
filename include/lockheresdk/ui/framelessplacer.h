#ifndef FRAMELESSPLACER_H
#define FRAMELESSPLACER_H

#include <QWidget>

class FramelessPlacer : public QWidget {
	Q_OBJECT
public:
	explicit FramelessPlacer(QWidget* parent = nullptr);
	~FramelessPlacer();

	//! 重置内容控件
	FramelessPlacer* setBackgroundColor(const QColor& bgColor);
	FramelessPlacer* setBorderRadius(int borderRadius);
	FramelessPlacer* setContent(QWidget* content);

	//! 拖拽支持
	void mousePressEvent(QMouseEvent* e) Q_DECL_OVERRIDE;
	void mouseReleaseEvent(QMouseEvent* e) Q_DECL_OVERRIDE;
	void mouseMoveEvent(QMouseEvent* e) Q_DECL_OVERRIDE;

public:
	static constexpr int shadowRadius = 4;	 //!< 阴影半径

private:
	struct FramelessPlacerPrivate;
	FramelessPlacerPrivate* data_;

signals:
};

#endif	 // FRAMELESSPLACER_H
