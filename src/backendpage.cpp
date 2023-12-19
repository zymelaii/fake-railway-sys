#include <QFile>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QSqlQuery>
#include <QDateTime>

#include "ui/backendpage.h"
#include "backendpage.h"
#include "databasemanager.h"
#include "popupmessage.h"

class BackendPagePrivate {
public:
    QStandardItemModel *model;

    BackendPagePrivate()
        : model(new QStandardItemModel) {}

    ~BackendPagePrivate() {
        delete model;
    }

    void updateScheduleData() {
        auto     &db = DatabaseManager::getInstance();
        QSqlQuery query{};
        bool      ok = db.execute(query, "select * from schedule;");
        assert(ok);
        model->clear();
        model->setHorizontalHeaderLabels({"列车号", "出发地", "目的地", "出发时间", "抵达时间", "票价", "总座位数"});
        while (query.next()) {
            QList<QStandardItem *> row{};
            for (int i = 0; i < model->columnCount(); ++i) {
                auto value = query.value(i).toString();
                auto item  = new QStandardItem(value);
                item->setTextAlignment(Qt::AlignCenter);
                row.append(item);
            }
            model->appendRow(row);
        }
    }
};

BackendPage::BackendPage(QWidget *parent)
    : QWidget(parent)
    , d{new BackendPagePrivate}
    , ui{new Ui::BackendPage} {
    ui->setupUi(this);

    if (QFile qss(":/qss/backend.qss"); qss.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream stream(&qss);
        setStyleSheet(stream.readAll());
        qss.close();
    }

    ui->tableView->setModel(d->model);

    connect(ui->btAdd, SIGNAL(clicked()), this, SLOT(tryAddScheduleRecord()));
    connect(ui->btRemove, SIGNAL(clicked()), this, SLOT(tryRemoveScheduleRecord()));
    connect(ui->btReturn, &QPushButton::clicked, this, [this] {
        emit shouldGoBack();
    });
}

BackendPage::~BackendPage() {
    delete d;
    delete ui;
}

void BackendPage::refresh() {
    d->updateScheduleData();
}

void BackendPage::tryAddScheduleRecord() {
    auto train_id       = ui->inputRecordValue[0]->text();
    auto departure      = ui->inputRecordValue[1]->text();
    auto destination    = ui->inputRecordValue[2]->text();
    auto departure_time = ui->inputRecordValue[3]->text();
    auto arrival_time   = ui->inputRecordValue[4]->text();
    auto ticket_price   = ui->inputRecordValue[5]->text();
    auto capacity       = ui->inputRecordValue[6]->text();

    bool ok = true;
    if (!isTrainIdValid(train_id)) {
        PopupMessage::spawn("duplicate train id", this);
        return;
    }
    if (departure.isEmpty()) {
        PopupMessage::spawn("invalid departure", this);
        return;
    }
    if (destination.isEmpty()) {
        PopupMessage::spawn("invalid destination", this);
        return;
    }
    if (!isDateValid(departure_time)) {
        PopupMessage::spawn("invalid departure time", this);
        return;
    }
    if (!isDateValid(arrival_time)) {
        PopupMessage::spawn("invalid arrival time", this);
        return;
    }
    if (auto price = ticket_price.toFloat(&ok); !ok || price <= 0) {
        PopupMessage::spawn("invalid ticket price", this);
        return;
    }
    if (auto total = capacity.toInt(&ok); !ok || total <= 0) {
        PopupMessage::spawn("invalid capacity", this);
        return;
    }

    auto     &db = DatabaseManager::getInstance();
    QSqlQuery query{};
    auto      sql = QString("insert into schedule values ('%1', '%2', '%3', '%4', '%5', '%6', '%7');")
                   .arg(train_id)
                   .arg(departure)
                   .arg(destination)
                   .arg(departure_time)
                   .arg(arrival_time)
                   .arg(ticket_price)
                   .arg(capacity);
    ok = db.execute(query, sql);
    assert(ok);

    for (auto &input : ui->inputRecordValue) { input->clear(); }
    refresh();
}

void BackendPage::tryRemoveScheduleRecord() {
    auto train_id = ui->inputTrainId->text();
    bool ok       = true;
    if (train_id.toInt(&ok); !ok) {
        PopupMessage::spawn("invalid train id", this);
        return;
    }
    if (!isTrainIdExists(train_id)) {
        PopupMessage::spawn("train id not exists", this);
        return;
    }

    auto     &db = DatabaseManager::getInstance();
    QSqlQuery query{};
    auto      sql = QString("delete from schedule where train_id = '%1';").arg(train_id);
    ok            = db.execute(query, sql);
    assert(ok);

    ui->inputTrainId->clear();
    refresh();
}

bool BackendPage::isTrainIdExists(const QString &id) {
    for (int i = 0; i < d->model->rowCount(); ++i) {
        if (d->model->item(i, 0)->text() == id) { return true; }
    }
    return false;
}

bool BackendPage::isTrainIdValid(const QString &id) {
    bool ok = true;
    if (id.toInt(&ok); !ok) { return false; }
    if (isTrainIdExists(id)) { return false; }
    return true;
}

bool BackendPage::isDateValid(const QString &date) {
    return QDateTime::fromString(date, "yyyy-MM-dd hh:mm").isValid();
}
