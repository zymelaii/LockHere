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
	explicit LineInput(QWidget* parent = nullptr);

	bool eventFilter(QObject* obj, QEvent* event) Q_DECL_OVERRIDE;
	void paintEvent(QPaintEvent* e) Q_DECL_OVERRIDE;
	void focusInEvent(QFocusEvent* e) Q_DECL_OVERRIDE;
	void focusOutEvent(QFocusEvent* e) Q_DECL_OVERRIDE;

	void setHint(const QString& hint);

protected slots:
	void onActivate();
	void onDeactivate();
	void onFocusNext();

signals:
	void activate();
	void deactivate();
	void textChanged(const QString&);

private:
	QPalette pal_;

	QPushButton* btOption;
	QLabel*		 lbHint;
	QLineEdit*	 leInput;
	QHBoxLayout* hloMain;
	QVBoxLayout* vloEdit;
};

}	// namespace utils
}	// namespace Ui
}	// namespace LockHere

#endif	 // LOCKHERE_UI_UTILS_LINEINPUT_H
