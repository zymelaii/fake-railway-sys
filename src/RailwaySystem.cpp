#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGraphicsBlurEffect>
#include <QPainter>
#include <QEvent>
#include <QMouseEvent>
#include <QFile>
#include <QDialog>
#include <QTimer>

#include "RailwaySystem.h"
#include "PopupMessage.h"

struct RailwaySystemPrivate {
    QLabel      *lbBackground;
    QLabel      *lbUsername;
    QLabel      *lbPassword;
    QLabel      *lbNoAccount;
    QPushButton *btLogin;
    QPushButton *btRegister;
    QLineEdit   *inputUsername;
    QLineEdit   *inputPassword;
    QLineEdit   *inputVerify;
    QWidget     *pane;
    QLabel      *spacer;
    QHBoxLayout *layoutLogin;
    QVBoxLayout *layoutPane;
    QLayout     *layoutBackground;
    QLabel      *lbMsg;
};

RailwaySystem::RailwaySystem(QWidget *parent)
    : QWidget(parent)
    , d{new RailwaySystemPrivate} {
    d->lbBackground = new QLabel;
    d->lbBackground->setPixmap(QPixmap(":/common/background.png"));
    d->lbBackground->setScaledContents(true);
    d->lbBackground->setFixedSize(640, 400);
    d->lbBackground->setContentsMargins({});
    d->layoutBackground = new QGridLayout(this);
    d->layoutBackground->addWidget(d->lbBackground);
    d->layoutBackground->setContentsMargins({});

    d->pane       = new QWidget;
    d->layoutPane = new QVBoxLayout(d->pane);
    d->pane->setObjectName("RailwaySystemLoginPane");
    d->pane->setAutoFillBackground(true);
    auto pal = d->pane->palette();
    pal.setColor(QPalette::Window, QColor(142, 79, 41, 200));
    d->pane->setPalette(pal);

    d->spacer      = new QLabel;
    d->layoutLogin = new QHBoxLayout(d->lbBackground);
    d->layoutLogin->setSpacing(0);
    d->layoutLogin->setContentsMargins(20, 0, 20, 0);

    d->lbUsername    = new QLabel("Username");
    d->lbPassword    = new QLabel("Password");
    d->lbNoAccount   = new QLabel("No Account");
    d->inputUsername = new QLineEdit;
    d->inputPassword = new QLineEdit;
    d->inputVerify   = new QLineEdit;
    d->btLogin       = new QPushButton("login");
    d->btRegister    = new QPushButton("register");
    d->lbNoAccount->setObjectName("RailwaySystemNoAccount");
    d->lbNoAccount->setAlignment(Qt::AlignRight);
    d->lbNoAccount->installEventFilter(this);
    d->inputUsername->setPlaceholderText("type your username");
    d->inputPassword->setPlaceholderText("type your password");
    d->inputPassword->setEchoMode(QLineEdit::Password);
    d->inputVerify->setPlaceholderText("verify your password");
    d->inputVerify->setEchoMode(QLineEdit::Password);

    d->lbMsg = new QLabel;

    if (QFile qss(":/qss/login.qss"); qss.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream stream(&qss);
        setStyleSheet(stream.readAll());
        qss.close();
    }

    setPage(Page::Login);

    connect(
        d->btRegister, SIGNAL(clicked(bool)), this, SLOT(postRegisterForm()));
}

RailwaySystem::~RailwaySystem() {
    delete d;
}

void RailwaySystem::setPage(Page page) {
    while (d->layoutLogin->count() > 0) { d->layoutLogin->takeAt(0); }
    while (d->layoutPane->count() > 0) {
        auto item = d->layoutPane->takeAt(0);
        if (auto w = item->widget()) { w->hide(); }
    }

    d->spacer->setFixedWidth(d->lbBackground->width() / 2);

    switch (page) {
        case Page::Login: {
            d->layoutLogin->addWidget(d->pane);
            d->layoutLogin->addWidget(d->spacer);
            d->layoutPane->addStretch();
            d->layoutPane->addWidget(d->lbUsername);
            d->layoutPane->addWidget(d->inputUsername);
            d->layoutPane->addWidget(d->lbPassword);
            d->layoutPane->addWidget(d->inputPassword);
            d->layoutPane->addWidget(d->lbNoAccount);
            d->layoutPane->addWidget(d->btLogin);
            d->layoutPane->addStretch();
        } break;
        case Page::Register: {
            d->layoutLogin->addWidget(d->spacer);
            d->layoutLogin->addWidget(d->pane);
            d->layoutPane->addStretch();
            d->layoutPane->addWidget(d->lbUsername);
            d->layoutPane->addWidget(d->inputUsername);
            d->layoutPane->addWidget(d->lbPassword);
            d->layoutPane->addWidget(d->inputPassword);
            d->layoutPane->addWidget(d->inputVerify);
            d->layoutPane->addWidget(d->btRegister);
            d->layoutPane->addStretch();
        } break;
    }

    for (int i = 0; i < d->layoutPane->count(); ++i) {
        auto item = d->layoutPane->itemAt(i);
        if (auto w = item->widget()) { w->show(); }
    }

    d->inputUsername->clear();
    d->inputPassword->clear();
    d->inputVerify->clear();
}

bool RailwaySystem::eventFilter(QObject *object, QEvent *e) {
    if (object == d->lbNoAccount && e->type() == QEvent::MouseButtonPress) {
        auto event = static_cast<QMouseEvent *>(e);
        if (event->button() == Qt::LeftButton) {
            setPage(Page::Register);
            return true;
        }
    }
    return QWidget::eventFilter(object, e);
}

void RailwaySystem::postRegisterForm() {
    auto username = d->inputUsername->text();
    auto password = d->inputPassword->text();
    auto verify   = d->inputVerify->text();
    if (username.isEmpty()) {
        PopupMessage::spawn("username is empty", this);
    } else if (password.length() < 8) {
        PopupMessage::spawn("password is less than 8 characters", this);
    } else if (password != verify) {
        PopupMessage::spawn("passwords do not match", this);
    } else {
        PopupMessage::spawn("account registration successful", this);
        setPage(Page::Login);
    }
}
