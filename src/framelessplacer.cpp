#include <lockheresdk/ui/utils/framelessplacer.h>

#include <QMouseEvent>
#include <QGridLayout>
#include <QGraphicsDropShadowEffect>
#include <QFile>
#include <QLabel>

#include <QDebug>

namespace LockHere {

struct FramelessPlacer::FramelessPlacerPrivate {
	FramelessPlacerPrivate()
		: placer_(new QWidget) {
		new QGridLayout(placer_);
	}

	~FramelessPlacerPrivate() {
		//! NOTE: placer_ implicitly becomes the children of FramelessPlacer,
		//> so there is no need to free it twice
	}

	void updatePlacer() {
		//! 应用边缘阴影效果
		auto effect = static_cast<QGraphicsDropShadowEffect*>(
			placer_->graphicsEffect() == nullptr ? new QGraphicsDropShadowEffect(placer_)
												 : placer_->graphicsEffect());
		effect->setBlurRadius(FramelessPlacer::shadowRadius);
		effect->setColor(bgColor_);
		effect->setOffset(0, 0);

		//! 应用 QSS
		placer_->setObjectName("this_FramelessPlacer");
		placer_->setStyleSheet(QString(R"(#this_FramelessPlacer {
            background: %1;
            border-radius: %2px;
        })")
								   .arg(bgColor_.name())
								   .arg(borderRadius_));
	}

	QWidget* setContent(QWidget* content) {
		QWidget* lastContent = nullptr;
		auto	 lo_grid	 = static_cast<QGridLayout*>(placer_->layout());
		if (lo_grid->count() > 0) {
			lastContent = lo_grid->itemAtPosition(0, 0)->widget();
			lo_grid->removeWidget(lastContent);
			lastContent->setParent(nullptr);
		}
		lo_grid->addWidget(content, 0, 0);
		lo_grid->setContentsMargins(borderRadius_, borderRadius_, borderRadius_, borderRadius_);
		return lastContent;
	}

	QPoint mousePressedPos_;   //!< 鼠标按下开始拖动时时的坐标
	QPoint winPosAsDrag_;	   //!< 窗口开始拖动时的原点坐标

	QWidget* placer_;		  //!< 载体控件
	int		 borderRadius_;	  //!< 载体圆角半径
	QColor	 bgColor_;		  //!< 载体背景色
};

FramelessPlacer::FramelessPlacer(QWidget* parent)
	: QWidget(parent)
	, data_(new FramelessPlacerPrivate) {
	//! 去除窗体边框
	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);

	//! 设置窗口背景透明
	setAttribute(Qt::WA_TranslucentBackground);

	setFocusPolicy(Qt::ClickFocus);

	auto lo_grid = new QGridLayout(this);
	lo_grid->addWidget(data_->placer_, 0, 0);
	lo_grid->setContentsMargins(shadowRadius, shadowRadius, shadowRadius, shadowRadius);
}

FramelessPlacer::~FramelessPlacer() {
	delete data_;
}

void FramelessPlacer::setBackgroundColor(const QColor& bgColor) {
	data_->bgColor_ = bgColor;
}

void FramelessPlacer::setBorderRadius(int borderRadius) {
	data_->borderRadius_ = borderRadius;
}

void FramelessPlacer::installStyleSheet(const QString& url) {
	QFile qssFile(url);
	qssFile.open(QIODevice::ReadOnly);
	if (qssFile.isOpen()) {
		setStyleSheet(qssFile.readAll());
		qssFile.close();
	}
}

QWidget* FramelessPlacer::setContent(QWidget* content) {
	//! 更新内容组件
	data_->updatePlacer();
	return data_->setContent(content);
}

void FramelessPlacer::mousePressEvent(QMouseEvent* e) {
	data_->mousePressedPos_ = e->globalPosition().toPoint();
	data_->winPosAsDrag_	= pos();
}

void FramelessPlacer::mouseReleaseEvent(QMouseEvent* e) {
	Q_UNUSED(e);

	data_->mousePressedPos_ = QPoint();
}

void FramelessPlacer::mouseMoveEvent(QMouseEvent* e) {
	if (!data_->mousePressedPos_.isNull()) {
		auto delta = e->globalPosition().toPoint() - data_->mousePressedPos_;
		move(data_->winPosAsDrag_ + delta);
	}
}

}	// namespace LockHere
