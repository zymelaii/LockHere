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
	void uiInitLogo();
	void uiInitMain();
	void setUnlockEnable(bool enabled);

signals:
    void tryUnlock(const QString& id, const QString& key);

private:
    QVBoxLayout*	  vloMain;
    QLabel*			  lbLogo;		//!< logo
    QFrame*			  frMain;		//!< mainFrame
    QLabel*			  lbWelcom;		//!< welcomeText
    utils::LineInput* liAccount;	//!< account
    utils::LineInput* liPassword;	//!< password
    QPushButton*	  btUnlock;		//!< unlock
};

}	// namespace Ui
}	// namespace LockHere
#endif	 // LOCKHERE_UI_LOGINWELCOME_H
