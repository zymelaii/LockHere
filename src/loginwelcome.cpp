#include <lockheresdk/ui/loginwelcome.h>

#include <lockheresdk/svghelper.h>
#include <QApplication>
#include <QFile>
#include <QStyle>

namespace LockHere {
namespace Ui {

LoginWelcome::LoginWelcome(QWidget* parent)
	: QWidget(parent) {
	//! init ui
	uiInitLogo();
	uiInitMain();

	//! complete layout
    auto layout = new QVBoxLayout(this);
	layout->addWidget(lbLogo);
	layout->addWidget(frMain);

	//! configure
	setFocusPolicy(Qt::NoFocus);

	//! connect signal & slot
	auto cbCheckUnlock = [this] {
		auto key = !liPassword->text().isEmpty() && !liAccount->text().isEmpty();
		if (key && !btUnlock->isEnabled()) {
			setUnlockEnable(true);
        } else if (!key && btUnlock->isEnabled()) {
			setUnlockEnable(false);
		}
	};

	connect(liAccount, &utils::LineInput::textChanged, this, cbCheckUnlock);
	connect(liPassword, &utils::LineInput::textChanged, this, cbCheckUnlock);

	connect(liPassword, &utils::LineInput::clicked, this, [this] {
		if (liPassword->echoMode() == QLineEdit::Password) {
			liPassword->setEchoMode(QLineEdit::Normal);
			liPassword->setOptionIcon(":/icons/eye.svg", QColor("#e1e1e1"));
		} else {
			liPassword->setEchoMode(QLineEdit::Password);
			liPassword->setOptionIcon(":/icons/eye-alt.svg", QColor("#e1e1e1"));
		}
	});
}

void LoginWelcome::uiInitLogo() {
	//! allocate
	lbLogo = new QLabel(this);

	//! init lbLogo
    lbLogo->setObjectName("logo");
	lbLogo->setPixmap(QPixmap(":/icons/logo.png").scaledToHeight(100, Qt::SmoothTransformation));
	lbLogo->setAlignment(Qt::AlignCenter);
}

void LoginWelcome::uiInitMain() {
	//! allocate
    frMain	   = new QFrame;
	vloMain	   = new QVBoxLayout(frMain);
	lbWelcom   = new QLabel(frMain);
	liAccount  = new utils::LineInput(frMain);
	liPassword = new utils::LineInput(frMain);
	btUnlock   = new QPushButton(frMain);

	//! init frMain
    frMain->setObjectName("mainFrame");
	frMain->setContentsMargins(-1, 8, -1, 8);

	//! init vloMain
	vloMain->setSpacing(16);

	//! init lbWelcom
    lbWelcom->setObjectName("welcomeText");
	lbWelcom->setAlignment(Qt::AlignHCenter);
	lbWelcom->setText("Welcom to use Lock Here!");

	//! init liAccount
    liAccount->setObjectName("account");
	liAccount->setHint("logged in as");
	liAccount->setOptionEnabled(true);
	liAccount->setOptionIcon(QStringLiteral(":/icons/more.svg"), QColor("#e1e1e1"));

	//! init liPassword
    liPassword->setObjectName("password");
	liPassword->setHint("enter master password");
	liPassword->setOptionEnabled(true);
	liPassword->setOptionIcon(QStringLiteral(":/icons/eye-alt.svg"), QColor("#e1e1e1"));
	liPassword->setEchoMode(QLineEdit::Password);

	//! init btUnlock
    btUnlock->setObjectName("unlock");
	btUnlock->setFixedHeight(32);
	setUnlockEnable(false);
	btUnlock->setText("Unlock");

	//! complete layout
	vloMain->addWidget(lbWelcom);
	vloMain->addWidget(liAccount);
	vloMain->addWidget(liPassword);
	vloMain->addWidget(btUnlock);
}

void LoginWelcome::setUnlockEnable(bool enabled) {
	auto w = btUnlock->fontInfo().pixelSize() + 4;

	QString	 svgPath(QStringLiteral(":/icons/lock-unlock.svg"));
	QSize	 icoSize(w, w);
	QColor	 currentColor(enabled ? "#dae0e3" : "#818283");
	QPixmap* pSvgPixmap = loadSvgAsPixmap(svgPath, icoSize, currentColor);

	btUnlock->setEnabled(enabled);
	btUnlock->setIcon(*pSvgPixmap);
	delete pSvgPixmap;
}

}	// namespace Ui
}	// namespace LockHere
