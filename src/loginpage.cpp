#include <QFile>
#include <QMouseEvent>

#include "ui/loginpage.h"
#include "loginpage.h"
#include "popupmessage.h"
#include "databasemanager.h"

class LoginPagePrivate {};

LoginPage::LoginPage(QWidget *parent)
    : QWidget(parent)
    , d{new LoginPagePrivate}
    , ui{new Ui::LoginPage} {
    ui->setupUi(this);

    if (QFile qss(":/qss/login.qss"); qss.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream stream(&qss);
        setStyleSheet(stream.readAll());
        qss.close();
    }

    ui->lbNoAccount->installEventFilter(this);
    ui->lbBack->installEventFilter(this);

    connect(ui->btLogin, SIGNAL(clicked(bool)), this, SLOT(submitLoginForm()));
    connect(ui->btRegister, SIGNAL(clicked(bool)), this, SLOT(submitRegisterForm()));
}

LoginPage::~LoginPage() {
    delete d;
    delete ui;
}

bool LoginPage::eventFilter(QObject *object, QEvent *e) {
    do {
        if (e->type() != QEvent::MouseButtonPress) { break; }
        auto event = static_cast<QMouseEvent *>(e);
        if (event->button() != Qt::LeftButton) { break; }
        if (object == ui->lbNoAccount) {
            switchToRegisterStage();
            return true;
        } else if (object == ui->lbBack) {
            switchToLoginStage();
            return true;
        }
    } while (false);
    return QWidget::eventFilter(object, e);
}

void LoginPage::resizeEvent(QResizeEvent *e) {
    auto w = ui->background->width() / 2;
    ui->spacer->setFixedWidth(w);
    ui->layoutBackground->setContentsMargins(w / 16, 0, w / 16, 0);
}

void LoginPage::switchToLoginStage() {
    if (ui->btLogin->isVisible()) { return; }
    ui->inputUsername->clear();
    ui->inputPassword->clear();
    ui->inputPasswordVerify->clear();
    ui->inputPasswordVerify->hide();
    ui->btLogin->show();
    ui->lbNoAccount->show();
    ui->lbBack->hide();
    ui->btRegister->hide();
    auto w = ui->layoutBackground->takeAt(0);
    Q_ASSERT(w->widget() == ui->spacer);
    ui->layoutBackground->addWidget(ui->spacer);
}

void LoginPage::switchToRegisterStage() {
    if (ui->btRegister->isVisible()) { return; }
    ui->inputUsername->clear();
    ui->inputPassword->clear();
    ui->inputPasswordVerify->clear();
    ui->inputPasswordVerify->show();
    ui->btLogin->hide();
    ui->lbNoAccount->hide();
    ui->lbBack->show();
    ui->btRegister->show();
    auto w = ui->layoutBackground->takeAt(0);
    Q_ASSERT(w->widget() == ui->container);
    ui->layoutBackground->addWidget(ui->container);
}

void LoginPage::submitLoginForm() {
    const auto &username = ui->inputUsername->text();
    const auto &password = ui->inputPassword->text();
    if (username.isEmpty()) {
        PopupMessage::spawn("username is empty", this);
        return;
    } else if (password.isEmpty()) {
        PopupMessage::spawn("password is empty", this);
        return;
    }
    auto exists = DatabaseManager::getInstance().validateOrCreateAccount(username, password, true);
    if (!exists) {
        PopupMessage::spawn("username or password is wrong", this);
    } else {
        emit loginPassed(username);
    }
}

void LoginPage::submitRegisterForm() {
    const auto &username = ui->inputUsername->text();
    const auto &password = ui->inputPassword->text();
    const auto &verify   = ui->inputPasswordVerify->text();
    if (username.isEmpty()) {
        PopupMessage::spawn("username is empty", this);
        return;
    } else if (password.length() < 8) {
        PopupMessage::spawn("password is less than 8 characters", this);
        return;
    } else if (password != verify) {
        PopupMessage::spawn("passwords do not match", this);
        return;
    }
    auto ok = DatabaseManager::getInstance().validateOrCreateAccount(username, password, false);
    if (!ok) {
        PopupMessage::spawn("account alreay exists", this);
        return;
    }
    PopupMessage::spawn("account registration successful", this);
    switchToLoginStage();
}
