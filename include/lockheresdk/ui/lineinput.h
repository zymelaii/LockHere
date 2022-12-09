#ifndef LOCKHERE_UI_UTILS_LINEINPUT_H
#define LOCKHERE_UI_UTILS_LINEINPUT_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QLayout>
#include <QPalette>

namespace LockHere {
namespace Ui {
namespace utils {

class LineInput : public QWidget {
	Q_OBJECT

public:
	using EchoMode = QLineEdit::EchoMode;

	explicit LineInput(QWidget* parent = nullptr);

	bool eventFilter(QObject* obj, QEvent* event) Q_DECL_OVERRIDE;
	void paintEvent(QPaintEvent* e) Q_DECL_OVERRIDE;
	void focusInEvent(QFocusEvent* e) Q_DECL_OVERRIDE;
	void focusOutEvent(QFocusEvent* e) Q_DECL_OVERRIDE;

	void	 setHint(const QString& hint);
	void	 setOptionEnabled(bool enabled);
	void	 setOptionIcon(const QPixmap& icon);
	void	 setOptionIcon(const QString& svgPath, QColor currentColor = Qt::black);
	EchoMode echoMode() const;
	void	 setEchoMode(EchoMode mode);
	QString	 text() const;

protected slots:
	void onActivate();
	void onDeactivate();
	void onFocusNext();

signals:
	void activate();
	void deactivate();
	void textChanged(const QString&);
	void clicked();

private:
	QPalette pal_;

	QLabel*		 lbHint;
	QLineEdit*	 leInput;
	QPushButton* btOption;
	QHBoxLayout* hloMain;
	QVBoxLayout* vloEdit;
};

}	// namespace utils
}	// namespace Ui
}	// namespace LockHere

#endif	 // LOCKHERE_UI_UTILS_LINEINPUT_H
