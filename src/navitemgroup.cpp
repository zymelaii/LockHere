#include <lockheresdk/ui/navitemgroup.h>


namespace LockHere {
namespace Ui {
namespace utils {

NavItemGroup::NavItemGroup(QWidget* parent)
    : QWidget{parent} {

    layout	= new QVBoxLayout(this);
    lbTitle = new QLabel(this);
    layout->addWidget(lbTitle);
    layout->addSpacing(4);

    lbTitle->hide();

    lastSelected_ = nullptr;
}

QString NavItemGroup::title() const {
    return lbTitle->text();
}

void NavItemGroup::setTitle(const QString& title) {
    lbTitle->setText(title);
    if (lbTitle->isHidden()) {
        lbTitle->show();
    }
}

int NavItemGroup::count() const {
    return layout->count() - 2;
}

void NavItemGroup::addItem(NavItem* item) {
    layout->addWidget(item);

    connect(item, &NavItem::selected, this, [this, item] {
        if (item == lastSelected_) return;
        if (lastSelected_ != nullptr) {
            lastSelected_->setSelected(false);
            lastSelected_->updateState();
        }
        if (!item->isExpandable()) {
            lastSelected_ = item;
        }
    });
}

NavItem* NavItemGroup::itemAt(int index) {
    auto item = layout->itemAt(index + 2);
    if (!item) return nullptr;
    return qobject_cast<NavItem*>(item->widget());
}

}	// namespace utils
}	// namespace Ui
}	// namespace LockHere
