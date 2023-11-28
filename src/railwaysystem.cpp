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

#include "ui/railwaysystem.h"
#include "railwaysystem.h"
#include "popupmessage.h"

class RailwaySystemPrivate {};

RailwaySystem::RailwaySystem(QWidget *parent)
    : QWidget(parent)
    , d{new RailwaySystemPrivate}
    , ui{new Ui::RailwaySystem} {
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

RailwaySystem::~RailwaySystem() {
    delete d;
    delete ui;
}

bool RailwaySystem::eventFilter(QObject *object, QEvent *e) {
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

void RailwaySystem::resizeEvent(QResizeEvent *e) {
    auto w = ui->background->width() / 2;
    ui->spacer->setFixedWidth(w);
    ui->layoutBackground->setContentsMargins(w / 16, 0, w / 16, 0);
}

void RailwaySystem::switchToLoginStage() {
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

void RailwaySystem::switchToRegisterStage() {
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

void RailwaySystem::submitLoginForm() {
    const auto &username = ui->inputUsername->text();
    const auto &password = ui->inputPassword->text();
    if (username.isEmpty()) {
        PopupMessage::spawn("username is empty", this);
    } else if (password.isEmpty()) {
        PopupMessage::spawn("password is empty", this);
    } else {
        PopupMessage::spawn("not implemented yet", this);
    }
}

void RailwaySystem::submitRegisterForm() {
    const auto &username = ui->inputUsername->text();
    const auto &password = ui->inputPassword->text();
    const auto &verify   = ui->inputPasswordVerify->text();
    if (username.isEmpty()) {
        PopupMessage::spawn("username is empty", this);
    } else if (password.length() < 8) {
        PopupMessage::spawn("password is less than 8 characters", this);
    } else if (password != verify) {
        PopupMessage::spawn("passwords do not match", this);
    } else {
        PopupMessage::spawn("account registration successful", this);
        switchToLoginStage();
    }
}
