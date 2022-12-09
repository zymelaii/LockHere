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
	void setUnlockEnable(bool enabled);

signals:
	void shouldMinimizeToTray();
	void shouldMinimize();
	void shouldClose();

private:
	QVBoxLayout* layout;

	QHBoxLayout* hloSysmenu;
	QPushButton* btMinimizeToTray;
	QPushButton* btMinimize;
	QPushButton* btClose;

	QLabel* lbLogo;

	QFrame*			  frMain;
	QVBoxLayout*	  vloMain;
	QLabel*			  lbWelcom;
	utils::LineInput* liAccount;
	utils::LineInput* liPassword;
	QPushButton*	  btUnlock;
};


}	// namespace Ui
}	// namespace LockHere
#endif	 // LOCKHERE_UI_LOGINWELCOME_H
