#include "loginwelcome.h"

#include <QApplication>
#include <QStyle>

namespace LockHere {
namespace Ui {

LoginWelcome::LoginWelcome(QWidget* parent)
	: QWidget(parent) {

	layout = new QVBoxLayout(this);

	//! init ui
	uiInitSysmenu();
	uiInitLogo();
	uiInitMain();

	//! complete layout
	layout->addLayout(hloSysmenu);
	layout->addWidget(lbLogo);
	layout->addWidget(frMain);

	//! connect signal & slot
	connect(btMinimize, &QPushButton::clicked, this, [this] { emit shouldMinimize(); });

	connect(btClose, &QPushButton::clicked, this, [this] { emit shouldClose(); });
}

void LoginWelcome::uiInitSysmenu() {
	//! allocate
	hloSysmenu = new QHBoxLayout;
	btMinimize = new QPushButton;
	btClose	   = new QPushButton;

	//! init hloSysmenu
	hloSysmenu->setAlignment(Qt::AlignRight);

	//! init btMinimize
	btMinimize->setObjectName("btMinimize");
	btMinimize->setStyleSheet(R"(
    #btMinimize {
        border: none;
        border-radius: 8px;
    }
    #btMinimize:!hover {
        background: #c7cacf;
    }
    #btMinimize::hover {
        background: #d9d9d9;
    })");
	btMinimize->setDefault(false);
	btMinimize->setFixedSize(16, 16);
	{
		auto icon = QApplication::style()->standardIcon(QStyle::SP_TitleBarMinButton);
		btMinimize->setIcon(icon.pixmap(QSize(8, 8)));
	}
	btMinimize->setFocusPolicy(Qt::NoFocus);

	//! init btClose
	btClose->setObjectName("btClose");
	btClose->setStyleSheet(R"(
    #btClose {
        border: none;
        border-radius: 8px;
    }
    #btClose:!hover {
        background: #c7cacf;
    }
    #btClose::hover {
        background: red;
    })");
	btClose->setDefault(false);
	btClose->setFixedSize(16, 16);
	{
		auto icon = QApplication::style()->standardIcon(QStyle::SP_TitleBarCloseButton);
		btClose->setIcon(icon.pixmap(QSize(8, 8)));
	}
	btClose->setFocusPolicy(Qt::NoFocus);

	//! complete layout
	hloSysmenu->addWidget(btMinimize);
	hloSysmenu->addWidget(btClose);
}

void LoginWelcome::uiInitLogo() {
	//! allocate
	lbLogo = new QLabel(this);

	//! init lbLogo
	lbLogo->setObjectName("lbLogo");
	lbLogo->setStyleSheet(R"(#lbLogo {
        background: transparent;
        color: white;
        border: none;
    })");
	{
		QFont font;
		font.setFamily(QString::fromUtf8("Aa叹书体 (非商业使用)"));
		font.setPointSize(32);
		lbLogo->setFont(font);
	}
	lbLogo->setAlignment(Qt::AlignHCenter);
	lbLogo->setText("Lock Here");
}

void LoginWelcome::uiInitMain() {
	//! allocate
	frMain	   = new QFrame;
	vloMain	   = new QVBoxLayout(frMain);
	lbWelcom   = new QLabel(frMain);
	liAccount  = new utils::LineInput;
	liPassword = new utils::LineInput;

	//! init frMain
	frMain->setObjectName("frMain");
	frMain->setStyleSheet(R"(#frMain {
        background: #252525;
        border-radius: 16px;
        border: 1px solid #303030;
    })");

	//! init vloMain
	vloMain->setSpacing(16);

	//! init lbWelcom
	lbWelcom->setObjectName("lbWelcom");
	lbWelcom->setStyleSheet(R"(#lbWelcom {
        background: transparent;
        color: #9a9a9a;
        border: none;
    })");
	lbWelcom->setAlignment(Qt::AlignHCenter);
	lbWelcom->setText("Welcom to use Lock Here!");

	//! init liAccount
	liAccount->setHint("logged in as");

	//! init liPassword
	liPassword->setHint("enter master password");

	//! complete layout
	vloMain->addWidget(lbWelcom);
	vloMain->addWidget(liAccount);
	vloMain->addWidget(liPassword);
}

}	// namespace Ui
}	// namespace LockHere
