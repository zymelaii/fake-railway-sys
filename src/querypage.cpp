#include <QFile>
#include <QDateTime>
#include <QSqlQuery>
#include <QPushButton>
#include <QLineEdit>
#include <QWidget>
#include <QLabel>

#include "ui/querypage.h"
#include "querypage.h"
#include "databasemanager.h"
#include "popupmessage.h"

class QueryPagePrivate {};

QueryPage::QueryPage(QWidget *parent)
    : QWidget(parent)
    , d{new QueryPagePrivate}
    , ui{new Ui::QueryPage} {
    ui->setupUi(this);

    if (QFile qss(":/qss/query.qss"); qss.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream stream(&qss);
        setStyleSheet(stream.readAll());
        qss.close();
    }

    connect(ui->btReturn, &QPushButton::clicked, this, [this] {
        emit shouldGoBack();
    });
}

QueryPage::~QueryPage() {
    delete d;
    delete ui;
}

void QueryPage::updateQueryResults(const QString &from, const QString &to, const QString &date) {
    auto dateTime = QDateTime::fromString(date, "yyyy/MM/dd");
    assert(dateTime.isValid());
    auto fmtDate = dateTime.toString("yyyy-MM-dd");
    auto nextDay = dateTime.addDays(1).toString("yyyy-MM-dd");

    ui->lbDate->setText(date);
    ui->lbRoute->setText(QString("%1 → %2").arg(from).arg(to));

    auto     &db = DatabaseManager::getInstance();
    QSqlQuery query{};
    auto      sql = QString(R"(
        select * from schedule
        where
            departure = '%1'
            and destination = '%2'
            and departure_time between '%3' and '%4'
        ;)")
                   .arg(from)
                   .arg(to)
                   .arg(fmtDate)
                   .arg(nextDay);
    auto ok = db.execute(query, sql);
    assert(ok);

    clearResults();
    while (query.next()) {
        ScheduleInfo schedule{};
        schedule.trainId       = query.value(0).toString();
        schedule.departureTime = query.value(3).toString();
        schedule.arrivalTime   = query.value(4).toString();
        schedule.ticketPrice   = query.value(5).toString();
        addResult(schedule);
    }
    ui->layoutResult->addStretch();
}

void QueryPage::clearResults() {
    while (ui->layoutResult->count() > 0) {
        auto w = ui->layoutResult->takeAt(0)->widget();
        if (!w) { continue; }
        w->hide();
        w->deleteLater();
    }
}

void QueryPage::addResult(const ScheduleInfo &schedule) {
    auto result            = new QWidget(ui->resultGroup);
    auto descContainer     = new QWidget(result);
    auto purchaseContainer = new QWidget(result);
    auto lbTrainId         = new QLabel(QString("%1 号列车").arg(schedule.trainId), descContainer);
    auto lbDesc            = new QLabel(descContainer);
    auto lbCost            = new QLabel(QString("￥%1").arg(schedule.ticketPrice), descContainer);
    auto inputNumber       = new QLineEdit(purchaseContainer);
    auto btPurchase        = new QPushButton("订票", purchaseContainer);

    auto layout = new QVBoxLayout(result);
    layout->addWidget(descContainer);
    layout->addWidget(purchaseContainer);

    auto layoutDesc = new QHBoxLayout(descContainer);
    layoutDesc->addWidget(lbTrainId);
    layoutDesc->addWidget(lbDesc);
    layoutDesc->addWidget(lbCost);

    auto layoutPurchase = new QHBoxLayout(purchaseContainer);
    layoutPurchase->addWidget(inputNumber);
    layoutPurchase->addWidget(btPurchase);

    layout->setContentsMargins({});
    layoutDesc->setContentsMargins({});
    layoutPurchase->setContentsMargins({});

    inputNumber->setPlaceholderText("购票数");

    lbDesc->setAlignment(Qt::AlignCenter);
    lbDesc->setMaximumHeight(lbDesc->fontMetrics().height());

    lbTrainId->setAlignment(Qt::AlignLeft);
    lbTrainId->setMaximumHeight(lbTrainId->fontMetrics().height());
    lbDesc->setAlignment(Qt::AlignCenter);
    lbDesc->setMaximumHeight(lbDesc->fontMetrics().height());
    lbCost->setAlignment(Qt::AlignRight);
    lbCost->setMaximumHeight(lbCost->fontMetrics().height());

    auto departDate = QDateTime::fromString(schedule.departureTime, "yyyy-MM-dd hh:mm");
    auto arriveDate = QDateTime::fromString(schedule.arrivalTime, "yyyy-MM-dd hh:mm");
    assert(departDate.isValid());
    assert(arriveDate.isValid());
    auto days = departDate.daysTo(arriveDate);
    auto desc = QString("%1 → %2%3")
                    .arg(departDate.toString("hh:mm"))
                    .arg(arriveDate.toString("hh:mm"))
                    .arg(days == 0 ? "" : QString("(+%1)").arg(days));
    lbDesc->setText(desc);

    connect(btPurchase, &QPushButton::clicked, this, [this, inputNumber, schedule] {
        bool ok  = false;
        auto num = inputNumber->text().toInt(&ok);
        if (!ok || num <= 0) {
            PopupMessage::spawn("invalid ticket number", this);
            return;
        }
        inputNumber->clear();
        emit purchaseRequest(schedule.trainId, num);
    });

    ui->layoutResult->addWidget(result);
}
