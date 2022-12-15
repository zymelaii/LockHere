#include <lockheresdk/ui/navigator.h>

#include <QPushButton>
#include <QLabel>
#include <QPaintEvent>
#include <QPainter>

namespace LockHere {
namespace Ui {
namespace utils {

Navigator::Navigator(QWidget* parent)
    : QWidget{parent} {

    layout			 = new QVBoxLayout(this);
    auto scrollProxy = new QScrollArea(this);
    scrollArea		 = new QWidget(this);
    loArea			 = new QVBoxLayout(scrollArea);

    scrollProxy->setWidget(scrollArea);
    scrollProxy->setWidgetResizable(true);
    scrollProxy->setFixedHeight(480);

    loArea->setSpacing(2);
    loArea->setContentsMargins(4, 4, 4, 4);
    loArea->setAlignment(Qt::AlignTop);

    layout->addWidget(scrollProxy);
}

NavItemGroup* Navigator::addGroup(const QString& groupName) {
    for (auto& e : groups_) {
        if (e->title() == groupName) {
            return e;
        }
    }

    auto group = new NavItemGroup(scrollArea);
    group->setTitle(groupName.toUpper());

    loArea->addWidget(group);
    groups_.append(group);

    return group;
}

NavItemGroup* Navigator::group(const QString& groupName) {
    auto name = groupName.toUpper();
    for (auto& e : groups_) {
        if (e->title() == name) {
            return e;
        }
    }
    return nullptr;
}

}	// namespace utils
}	// namespace Ui
}	// namespace LockHere
