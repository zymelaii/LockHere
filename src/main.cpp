#include <lockheresdk/ui/utils/framelessplacer.h>
#include <lockheresdk/ui/sysmenubar.h>
#include <lockheresdk/ui/loginwelcome.h>
#include <lockheresdk/ui/navigator.h>
#include <lockheresdk/ui/utils/svghelper.h>

#include <QApplication>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QFontDatabase>
#include <memory>

using namespace LockHere::Ui;
using LockHere::FramelessPlacer;
using LockHere::Ui::utils::NavItem;
using LockHere::Ui::utils::NavItemGroup;
using LockHere::Ui::utils::Navigator;

void setupTrayApp(FramelessPlacer* app) {
	auto actionQuit = new QAction(QStringLiteral("&Quit"));
	auto menu		= new QMenu(app);
	menu->addAction(actionQuit);

	auto trayIcon = new QSystemTrayIcon(app);
	trayIcon->setIcon(QIcon(":/icons/app.ico"));
	trayIcon->setContextMenu(menu);

	QObject::connect(actionQuit, &QAction::triggered, app, &FramelessPlacer::shouldClose);

	using Reason = QSystemTrayIcon::ActivationReason;
	QObject::connect(
		trayIcon, &QSystemTrayIcon::activated, app, [&self = *app, trayIcon](Reason reason) {
			if (reason == Reason::Trigger) {
				self.setWindowFlags(self.windowFlags() & ~Qt::ToolTip);
				self.show();
				trayIcon->hide();
			}
		});

	QObject::connect(app, &FramelessPlacer::shouldMinimizeToTray, app, [&self = *app, trayIcon] {
		self.setWindowFlags(self.windowFlags() | Qt::Tool);
		trayIcon->show();
	});

	QObject::connect(app, &FramelessPlacer::shouldMinimize, app, [&self = *app] {
		self.setWindowState(self.windowState() | Qt::WindowMinimized);
	});
}

int main(int argc, char* argv[]) {
	QApplication a(argc, argv);
	QFontDatabase::addApplicationFont(":/font/OpenSans.ttf");

	auto delegator = std::make_unique<FramelessPlacer>();

	auto contentUi	= new QWidget;
	auto vloContent = new QVBoxLayout(contentUi);
	auto sysMenuBar = new SysMenuBar(contentUi);
	auto welcomeUi	= new Navigator(contentUi);

	vloContent->setSpacing(0);
	vloContent->addWidget(sysMenuBar);

	//! add logo
	{
		auto logo = new QLabel(welcomeUi);
		logo->setFixedWidth(256);
		logo->setPixmap(QPixmap(":/icons/logo.png")
							.scaledToWidth(logo->width() * 0.8, Qt::SmoothTransformation));
		logo->setFixedHeight(logo->pixmap().height());
		logo->setAlignment(Qt::AlignCenter);
		vloContent->addWidget(logo);
	}

	vloContent->addWidget(welcomeUi);

	welcomeUi->addGroup("Vaults & Items");
	welcomeUi->addToGroup("Vaults & Items", "All Vaults", ":/icons/album.svg", "1");
	welcomeUi->addToGroup("Vaults & Items", "Attachments", ":/icons/attachment.svg", "3");
	welcomeUi->addToGroup("Vaults & Items", "Tags", ":/icons/tag.svg");
	welcomeUi->addToGroup("Vaults & Items", "Albums", ":/icons/album.svg", "12");

	//! connect signal & slot
	{
		QObject::connect(sysMenuBar,
						 &SysMenuBar::shouldMinimizeToTray,
						 delegator.get(),
						 &FramelessPlacer::shouldMinimizeToTray);
		QObject::connect(sysMenuBar,
						 &SysMenuBar::shouldMinimize,
						 delegator.get(),
						 &FramelessPlacer::shouldMinimize);
		QObject::connect(
			sysMenuBar, &SysMenuBar::shouldClose, delegator.get(), &FramelessPlacer::shouldClose);
		QObject::connect(delegator.get(), &FramelessPlacer::shouldClose, &a, [&] { a.quit(); });
	}

	//! setup
	{
		delegator->setBorderRadius(16);
		delegator->setBackgroundColor(QColor(32, 32, 32));
		delegator->setContent(contentUi);
		delegator->installStyleSheet(":/qss/dark.qss");
		delegator->setMinimumWidth(320);
		delegator->show();

		setupTrayApp(delegator.get());
	}

	return a.exec();
}
