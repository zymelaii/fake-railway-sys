#include <QFile>
#include <QDateTime>

#include "ui/mainpage.h"
#include "mainpage.h"
#include "popupmessage.h"

class MainPagePrivate {};

MainPage::MainPage(QWidget *parent)
    : QWidget(parent)
    , d{new MainPagePrivate}
    , ui{new Ui::MainPage} {
    ui->setupUi(this);

    if (QFile qss(":/qss/main.qss"); qss.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream stream(&qss);
        setStyleSheet(stream.readAll());
        qss.close();
    }

    connect(ui->btSwap, SIGNAL(clicked()), this, SLOT(swapRoute()));
    connect(ui->btQuery, SIGNAL(clicked()), this, SLOT(submitQueryForm()));
    connect(ui->btBackend, &QPushButton::clicked, this, [this] {
        emit shouldGotoBackend();
    });
    connect(ui->btMine, &QPushButton::clicked, this, [this] {
        emit shouldGotoMine();
    });
}

MainPage::~MainPage() {
    delete d;
    delete ui;
}

void MainPage::swapRoute() {
    auto src = ui->inputSrc->text();
    ui->inputSrc->setText(ui->inputDst->text());
    ui->inputDst->setText(src);
}

void MainPage::submitQueryForm() {
    auto src  = ui->inputSrc->text();
    auto dst  = ui->inputDst->text();
    auto date = ui->inputDate->text();

    if (src.isEmpty() || dst.isEmpty()) {
        PopupMessage::spawn("incomplete route", this);
        return;
    }

    if (!QDateTime::fromString(date, "yyyy/MM/dd").isValid()) {
        PopupMessage::spawn("invalid date", this);
        return;
    }

    emit queryRequest(src, dst, date);
}
