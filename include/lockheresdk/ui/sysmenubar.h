#ifndef LOCKHERE_UI_SYSMENUBAR_H
#define LOCKHERE_UI_SYSMENUBAR_H

#include <QWidget>
#include <QLayout>
#include <QPushButton>

namespace LockHere {
namespace Ui {

class SysMenuBar : public QWidget {
    Q_OBJECT

public:
    explicit SysMenuBar(QWidget* parent = nullptr);

protected:
    void uiInit();

signals:
    void shouldMinimizeToTray();
    void shouldMinimize();
    void shouldClose();

private:
    QPushButton* btMinimizeToTray;	 //!< minimizeToTray
    QPushButton* btMinimize;		 //!< minimize
    QPushButton* btClose;			 //!< close
};

}	// namespace Ui
}	// namespace LockHere
#endif	 // LOCKHERE_UI_LOGINWELCOME_H
