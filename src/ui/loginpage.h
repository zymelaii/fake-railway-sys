#pragma once

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>

namespace Ui {

class LoginPage {
public:
    QLabel      *background;
    QLabel      *lbUsernameInput;
    QLabel      *lbPasswordInput;
    QLabel      *lbNoAccount;
    QLabel      *lbBack;
    QWidget     *hintLinkContainer;
    QLineEdit   *inputUsername;
    QLineEdit   *inputPassword;
    QLineEdit   *inputPasswordVerify;
    QPushButton *btLogin;
    QPushButton *btRegister;
    QWidget     *container;
    QLabel      *spacer;
    QGridLayout *layout;
    QHBoxLayout *layoutBackground;
    QVBoxLayout *layoutContainer;
    QHBoxLayout *layoutHintLink;

    void setupUi(QWidget *loginPage) {
        if (loginPage->objectName().isEmpty()) { loginPage->setObjectName("LoginPage"); }

        //! widget
        background          = new QLabel(loginPage);
        container           = new QWidget(background);
        spacer              = new QLabel(background);
        lbUsernameInput     = new QLabel("Username", container);
        lbPasswordInput     = new QLabel("Password", container);
        hintLinkContainer   = new QWidget(container);
        lbNoAccount         = new QLabel("No Account", hintLinkContainer);
        lbBack              = new QLabel("Back", hintLinkContainer);
        inputUsername       = new QLineEdit(container);
        inputPassword       = new QLineEdit(container);
        inputPasswordVerify = new QLineEdit(container);
        btLogin             = new QPushButton("login", container);
        btRegister          = new QPushButton("register", container);

        //! layout
        layout = new QGridLayout(loginPage);
        layout->addWidget(background);

        layoutBackground = new QHBoxLayout(background);
        layoutBackground->addWidget(container);
        layoutBackground->addWidget(spacer);

        layoutContainer = new QVBoxLayout(container);
        layoutContainer->addStretch();
        layoutContainer->addWidget(lbUsernameInput);
        layoutContainer->addWidget(inputUsername);
        layoutContainer->addWidget(lbPasswordInput);
        layoutContainer->addWidget(inputPassword);
        layoutContainer->addWidget(inputPasswordVerify);
        layoutContainer->addWidget(hintLinkContainer);
        layoutContainer->addWidget(btLogin);
        layoutContainer->addWidget(btRegister);
        layoutContainer->addStretch();

        layoutHintLink = new QHBoxLayout(hintLinkContainer);
        layoutHintLink->addStretch();
        layoutHintLink->addWidget(lbNoAccount);
        layoutHintLink->addWidget(lbBack);

        //! property
        background->setPixmap(QPixmap(":/common/background.png"));
        background->setScaledContents(true);
        background->setMinimumSize(640, 400);

        container->setObjectName("Container");
        container->setAutoFillBackground(true);
        auto pal = container->palette();
        pal.setColor(QPalette::Window, QColor(142, 79, 41, 200));
        container->setPalette(pal);

        auto w = background->width() / 2;
        spacer->setFixedWidth(w);

        inputUsername->setPlaceholderText("type your username");

        inputPassword->setPlaceholderText("type your password");
        inputPassword->setEchoMode(QLineEdit::Password);

        inputPasswordVerify->setPlaceholderText("verify your password");
        inputPasswordVerify->setEchoMode(QLineEdit::Password);
        inputPasswordVerify->hide();

        hintLinkContainer->setContentsMargins({});
        hintLinkContainer->setObjectName("HintLinkContainer");

        lbNoAccount->setAlignment(Qt::AlignRight);

        lbBack->setAlignment(Qt::AlignRight);
        lbBack->hide();

        btRegister->hide();

        layout->setContentsMargins({});

        layoutBackground->setSpacing(0);
        layoutBackground->setContentsMargins(w / 16, 0, w / 16, 0);

        layoutHintLink->setContentsMargins({});
    }
};

} // namespace Ui
