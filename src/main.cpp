#include <lockheresdk/ui/framelessplacer.h>
#include <lockheresdk/ui/sysmenubar.h>
#include <lockheresdk/ui/loginwelcome.h>
#include <lockheresdk/svghelper.h>

#include <QApplication>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QFontDatabase>
#include <memory>


struct I : public QLineEdit {
    explicit I(QWidget* parent = nullptr)
        : QLineEdit{parent} {}

    void focusInEvent(QFocusEvent* e) Q_DECL_OVERRIDE {
        qDebug() << ((QWidget*)parent())->hasFocus() << hasFocus();
    }

    void focusOutEvent(QFocusEvent* e) Q_DECL_OVERRIDE {
        qDebug() << ((QWidget*)parent())->hasFocus() << hasFocus();
    }
};

void setupTrayApp(FramelessPlacer* app) {
    auto actionQuit = new QAction(QStringLiteral("&Quit"));
    auto menu		= new QMenu(app);
    menu->addAction(actionQuit);

    auto trayIcon = new QSystemTrayIcon(app);
    trayIcon->setIcon(QIcon(":/icons/app.ico"));
    trayIcon->setContextMenu(menu);

    QObject::connect(actionQuit, &QAction::triggered, app, &FramelessPlacer::shouldClose);

    using Reason = QSystemTrayIcon::ActivationReason;
    QObject::connect(trayIcon, &QSystemTrayIcon::activated, app, [&](Reason reason) {
        if (reason == Reason::Trigger) {
            app->setWindowFlags(app->windowFlags() & ~Qt::ToolTip);
            app->show();
            trayIcon->hide();
        }
    });

    QObject::connect(app, &FramelessPlacer::shouldMinimizeToTray, app, [&] {
        app->setWindowFlags(app->windowFlags() | Qt::Tool);
        trayIcon->show();
    });

    QObject::connect(app, &FramelessPlacer::shouldMinimize, app, [&] {
        app->setWindowState(app->windowState() | Qt::WindowMinimized);
    });
}

int main(int argc, char* argv[]) {
	QApplication a(argc, argv);
    QFontDatabase::addApplicationFont(":/font/OpenSans.ttf");

    auto delegator = std::make_unique<FramelessPlacer>();

    auto contentUi	= new QWidget;
    auto vloContent = new QVBoxLayout(contentUi);
    auto sysMenuBar = new LockHere::Ui::SysMenuBar(contentUi);
    auto welcomeUi	= new LockHere::Ui::LoginWelcome(contentUi);
    vloContent->setSpacing(0);
    vloContent->addWidget(sysMenuBar);
    vloContent->addWidget(welcomeUi);

    QObject::connect(sysMenuBar,
                     &LockHere::Ui::SysMenuBar::shouldMinimizeToTray,
                     delegator.get(),
                     &FramelessPlacer::shouldMinimizeToTray);
    QObject::connect(sysMenuBar,
                     &LockHere::Ui::SysMenuBar::shouldMinimize,
                     delegator.get(),
                     &FramelessPlacer::shouldMinimize);
    QObject::connect(sysMenuBar,
                     &LockHere::Ui::SysMenuBar::shouldClose,
                     delegator.get(),
                     &FramelessPlacer::shouldClose);
    QObject::connect(delegator.get(), &FramelessPlacer::shouldClose, &a, [&] { a.quit(); });

    delegator->setBorderRadius(16)
        ->setBackgroundColor(QColor(32, 32, 32))
        ->installStyleSheet(":/qss/dark.qss")
        ->setContent(contentUi)
        ->setFixedWidth(512);
    delegator->show();

    setupTrayApp(delegator.get());

    return a.exec();
}
