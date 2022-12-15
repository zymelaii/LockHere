#ifndef LOCKHERE_UI_UTILS_NAVITEMGROUP_H
#define LOCKHERE_UI_UTILS_NAVITEMGROUP_H

#include <lockheresdk/ui/navitem.h>
#include <QWidget>
#include <QLayout>
#include <QLabel>

namespace LockHere {
namespace Ui {
namespace utils {

class NavItemGroup : public QWidget {
    Q_OBJECT
public:
    explicit NavItemGroup(QWidget* parent = nullptr);

    QString title() const;
    void	setTitle(const QString& title);

    int count() const;

    void							 addItem(NavItem* item);
    template <typename... Args> void addItem(Args... args) { addItem(new NavItem(args..., this)); }

    NavItem* itemAt(int index);

signals:

private:
    QVBoxLayout* layout;
    QLabel*		 lbTitle;

    NavItem* lastSelected_;
};

}	// namespace utils
}	// namespace Ui
}	// namespace LockHere

#endif	 // LOCKHERE_UI_UTILS_NAVITEMGROUP_H
