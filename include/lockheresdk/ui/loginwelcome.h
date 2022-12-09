#ifndef LOCKHERE_UI_LOGINWELCOME_H
#define LOCKHERE_UI_LOGINWELCOME_H

#include <QWidget>
#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QLayout>

#include <lockheresdk/ui/lineinput.h>

namespace LockHere {
namespace Ui {

class LoginWelcome : public QWidget {
	Q_OBJECT

public:
	explicit LoginWelcome(QWidget* parent = nullptr);

private:
	void uiInitSysmenu();
	void uiInitLogo();
	void uiInitMain();

signals:
	void shouldMinimize();
	void shouldClose();

private:
	QVBoxLayout* layout;

	QHBoxLayout* hloSysmenu;
	QPushButton* btMinimize;
	QPushButton* btClose;

	QLabel* lbLogo;

	QFrame*			  frMain;
	QVBoxLayout*	  vloMain;
	QLabel*			  lbWelcom;
	utils::LineInput* liAccount;
	utils::LineInput* liPassword;
};


}	// namespace Ui
}	// namespace LockHere
#endif	 // LOCKHERE_UI_LOGINWELCOME_H
