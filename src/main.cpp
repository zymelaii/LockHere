#include <lockheresdk/ui/framelessplacer.h>
#include <lockheresdk/ui/loginwelcome.h>

#include <QApplication>
#include <QObject>
#include <memory>

int main(int argc, char* argv[]) {
	QApplication a(argc, argv);

	auto welcomUi = new LockHere::Ui::LoginWelcome;
	auto placer	  = std::make_unique<FramelessPlacer>();
	placer->setBackgroundColor(QColor(32, 32, 32))
		->setBorderRadius(16)
		->setContent(welcomUi)
		->show();

	QObject::connect(welcomUi, &LockHere::Ui::LoginWelcome::shouldClose, &a, &QApplication::quit);

	QObject::connect(
		welcomUi, &LockHere::Ui::LoginWelcome::shouldMinimize, placer.get(), [app = placer.get()] {
			app->setWindowState(Qt::WindowMinimized);
		});

	return a.exec();
}
