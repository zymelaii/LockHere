#include <lockheresdk/ui/sysmenubar.h>

#include <lockheresdk/ui/utils/svghelper.h>

namespace LockHere {
namespace Ui {

SysMenuBar::SysMenuBar(QWidget* parent)
    : QWidget{parent} {
    //! setup ui
    uiInit();
    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);

    //! connect signal & slot
    connect(btMinimizeToTray, &QPushButton::clicked, this, &SysMenuBar::shouldMinimizeToTray);
    connect(btMinimize, &QPushButton::clicked, this, &SysMenuBar::shouldMinimize);
    connect(btClose, &QPushButton::clicked, this, &SysMenuBar::shouldClose);
}

void SysMenuBar::uiInit() {
    //! allocate
    auto layout		 = new QHBoxLayout(this);
    btMinimizeToTray = new QPushButton(this);
    btMinimize		 = new QPushButton(this);
    btClose			 = new QPushButton(this);

    //! init layout
    layout->setAlignment(Qt::AlignRight);

    //! init btMinimizeToTray
    btMinimizeToTray->setObjectName("minimizeToTray");
    btMinimizeToTray->setDefault(false);
    btMinimizeToTray->setFixedSize(16, 16);
    {
        auto pSvgPixmap = loadSvgAsPixmap(QStringLiteral(":/icons/minimize-alt.svg"),
                                          btMinimizeToTray->size() - QSize(4, 4),
                                          Qt::white);
        btMinimizeToTray->setIcon(*pSvgPixmap);
        delete pSvgPixmap;
    }
    btMinimizeToTray->setFocusPolicy(Qt::NoFocus);

    //! init btMinimize
    btMinimize->setObjectName("minimize");
    btMinimize->setDefault(false);
    btMinimize->setFixedSize(16, 16);
    {
        auto pSvgPixmap = loadSvgAsPixmap(
            QStringLiteral(":/icons/minimize.svg"), btMinimize->size() - QSize(4, 4), Qt::white);
        btMinimize->setIcon(*pSvgPixmap);
        delete pSvgPixmap;
    }
    btMinimize->setFocusPolicy(Qt::NoFocus);

    //! init btClose
    btClose->setObjectName("close");
    btClose->setDefault(false);
    btClose->setFixedSize(16, 16);
    {
        auto pSvgPixmap = loadSvgAsPixmap(
            QStringLiteral(":/icons/close.svg"), btClose->size() - QSize(4, 4), Qt::white);
        btClose->setIcon(*pSvgPixmap);
        delete pSvgPixmap;
    }
    btClose->setFocusPolicy(Qt::NoFocus);

    //! complete layout
    layout->addWidget(btMinimizeToTray);
    layout->addWidget(btMinimize);
    layout->addWidget(btClose);
}

}	// namespace Ui
}	// namespace LockHere
