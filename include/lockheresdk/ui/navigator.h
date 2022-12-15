#ifndef LOCKHERE_UI_UTILS_NAVIGATOR_H
#define LOCKHERE_UI_UTILS_NAVIGATOR_H

#include <lockheresdk/ui/navitem.h>
#include <lockheresdk/ui/navitemgroup.h>

#include <QWidget>
#include <QTreeWidget>
#include <QScrollArea>
#include <QListWidget>
#include <QLayout>
#include <QVector>
#include <QMap>

namespace LockHere {
namespace Ui {
namespace utils {

class Navigator : public QWidget {
    Q_OBJECT

public:
    explicit Navigator(QWidget* parent = nullptr);

    template <typename... Args> bool addToGroup(const QString& groupName, Args... args) {
        auto g = group(groupName);
        if (!g) return false;
        g->addItem(args...);
        return true;
    }

    NavItemGroup* addGroup(const QString& groupName);
    NavItemGroup* group(const QString& groupName);

signals:

private:
    QVBoxLayout *layout, *loArea;

    QMap<QString, QVBoxLayout*>		 vloGroupList_;
    QMap<QString, QVector<QWidget*>> view;

    QList<NavItemGroup*> groups_;

    QWidget*	 scrollArea;   //!< main field
    QHBoxLayout* hloPlugin;	   //!< plugin field
};

}	// namespace utils
}	// namespace Ui
}	// namespace LockHere

#endif	 // LOCKHERE_UI_UTILS_NAVIGATOR_H
