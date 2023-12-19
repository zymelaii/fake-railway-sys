#include <QFile>
#include <assert.h>
#include <QDateTime>
#include <QSqlQuery>

#include "ui/userpage.h"
#include "userpage.h"
#include "databasemanager.h"

class UserPagePrivate {
public:
    QString username;
    int     userId;
};

UserPage::UserPage(QWidget *parent)
    : QWidget(parent)
    , d{new UserPagePrivate}
    , ui{new Ui::UserPage} {
    ui->setupUi(this);

    if (QFile qss(":/qss/user.qss"); qss.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream stream(&qss);
        setStyleSheet(stream.readAll());
        qss.close();
    }

    connect(ui->btReturn, &QPushButton::clicked, this, [this] {
        emit shouldGoBack();
    });
}

UserPage::~UserPage() {
    delete d;
    delete ui;
}

void UserPage::updateByUserName(int userId, const QString &username) {
    assert(!username.isEmpty());
    d->userId   = userId;
    d->username = username;
    ui->lbUserName->setText(QString("你好，%1！").arg(username));
    refresh();
}

void UserPage::refresh() {
    while (ui->layoutTicket->count() > 0) {
        auto w = ui->layoutTicket->takeAt(0)->widget();
        if (!w) { continue; }
        w->hide();
        w->deleteLater();
    }

    auto     &db = DatabaseManager::getInstance();
    QSqlQuery query{};
    auto      sql = QString(R"(
        select
            t.train_id,
            t.ticket_id,
            s.departure_time,
            coalesce(i.group_size, 1) as total
        from tickets t
        natural join (select train_id, departure_time from schedule) s
        left join integrate i on t.ticket_id = i.ticket_id
        where t.user_id = '%1';)")
                   .arg(d->userId);
    auto ok = db.execute(query, sql);
    assert(ok);

    while (query.next()) {
        TicketInfo info{};
        info.trainId       = query.value(0).toString();
        info.ticketId      = query.value(1).toString();
        info.departureTime = query.value(2).toString();
        info.number        = query.value(3).toString();
        addTicketInfo(info);
    }

    ui->layoutTicket->addStretch();
}

void UserPage::addTicketInfo(const TicketInfo &ticket) {
    auto record        = new QWidget(ui->ticketGroup);
    auto descContainer = new QWidget(record);

    auto lbTrainId    = new QLabel(QString("%1 号列车").arg(ticket.trainId), descContainer);
    auto lbDepartTime = new QLabel(descContainer);
    auto btRefund     = new QPushButton("退票", record);

    auto layout = new QHBoxLayout(record);
    layout->addWidget(descContainer);
    layout->addWidget(btRefund);

    auto layoutDesc = new QVBoxLayout(descContainer);
    layoutDesc->addWidget(lbTrainId);
    layoutDesc->addWidget(lbDepartTime);

    layout->setContentsMargins({});
    layoutDesc->setContentsMargins({});

    lbTrainId->setAlignment(Qt::AlignLeft);
    lbTrainId->setMaximumHeight(lbTrainId->fontMetrics().height());
    lbDepartTime->setAlignment(Qt::AlignLeft);
    lbDepartTime->setMaximumHeight(lbDepartTime->fontMetrics().height());

    bool ok       = false;
    auto datetime = QDateTime::fromString(ticket.departureTime, "yyyy-MM-dd hh:mm");

    lbDepartTime->setText(QString("发车时间 %1 （%2）")
                              .arg(ticket.departureTime)
                              .arg(datetime > QDateTime::currentDateTime() ? "待登车" : "已过期"));

    if (auto number = ticket.number.toInt(&ok); ok && number > 1) {
        auto lbTicketInfo = new QLabel(QString("团队票 %1 人").arg(number), descContainer);
        layoutDesc->addWidget(lbTicketInfo);
        lbTicketInfo->setMaximumHeight(lbTicketInfo->fontMetrics().height());
    }

    connect(btRefund, &QPushButton::clicked, this, [this, ticketId = ticket.ticketId] {
        emit refundRequest(ticketId);
    });

    ui->layoutTicket->addWidget(record);
}
