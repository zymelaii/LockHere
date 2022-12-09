#include <lockheresdk/ui/framelessplacer.h>
#include <lockheresdk/ui/loginwelcome.h>

#include <QApplication>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QPainter>
#include <QFontDatabase>
#include <QSvgGenerator>
#include <memory>

int main(int argc, char* argv[]) {
	QApplication a(argc, argv);

	QFontDatabase::addApplicationFont(":/font/OpenSans.ttf");

	auto welcomeUi = new LockHere::Ui::LoginWelcome;
	auto placer	   = std::make_unique<FramelessPlacer>();
	placer->setBackgroundColor(QColor(32, 32, 32))
		->setBorderRadius(16)
		->setContent(welcomeUi)
		->show();

	auto actionQuit = new QAction(QStringLiteral("&Quit"));
	auto menu		= new QMenu(placer.get());
	menu->addAction(actionQuit);

	auto trayIcon = new QSystemTrayIcon(placer.get());
	trayIcon->setIcon(a.windowIcon());
	trayIcon->setContextMenu(menu);

	QObject::connect(
		actionQuit, &QAction::triggered, welcomeUi, &LockHere::Ui::LoginWelcome::shouldClose);

	using Reason = QSystemTrayIcon::ActivationReason;
	QObject::connect(trayIcon,
					 &QSystemTrayIcon::activated,
					 placer.get(),
					 [w = placer.get(), &trayIcon](Reason reason) {
						 if (reason == Reason::Trigger) {
							 w->setWindowFlags(w->windowFlags() & ~Qt::ToolTip);
							 w->show();
							 trayIcon->hide();
						 }
					 });

	QObject::connect(welcomeUi,
					 &LockHere::Ui::LoginWelcome::shouldMinimizeToTray,
					 placer.get(),
					 [w = placer.get(), &trayIcon] {
						 qDebug() << "trigger LockHere::Ui::LoginWelcome::shouldMinimizeToTray\n";
						 w->setWindowFlags(w->windowFlags() | Qt::Tool);
						 trayIcon->show();
					 });

	QObject::connect(
		welcomeUi, &LockHere::Ui::LoginWelcome::shouldMinimize, placer.get(), [w = placer.get()] {
			w->setWindowState(w->windowState() | Qt::WindowMinimized);
		});

	QObject::connect(
		welcomeUi, &LockHere::Ui::LoginWelcome::shouldClose, &a, [&a, &trayIcon, &welcomeUi] {
			trayIcon->hide();
			welcomeUi->hide();
			a.quit();
		});

	return a.exec();
}
