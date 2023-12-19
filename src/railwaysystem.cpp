#include <QHBoxLayout>
#include <QPushButton>
#include <QSqlQuery>
#include <assert.h>
#include <map>
#include <QDate>

#include "railwaysystem.h"
#include "loginpage.h"
#include "mainpage.h"
#include "querypage.h"
#include "backendpage.h"
#include "userpage.h"
#include "popupmessage.h"
#include "databasemanager.h"

class RailwaySystemPrivate {
public:
    QHBoxLayout *layout      = nullptr;
    QWidget     *currentPage = nullptr;
    LoginPage   *loginPage   = nullptr;
    MainPage    *mainPage    = nullptr;
    QueryPage   *queryPage   = nullptr;
    BackendPage *backendPage = nullptr;
    UserPage    *userPage    = nullptr;

    using PageMap = std::map<RailwaySystem::SystemPage, QWidget *>;
    PageMap pageMap;

    QString currentUser;
};

RailwaySystem::RailwaySystem(QWidget *parent)
    : QWidget(parent)
    , d{new RailwaySystemPrivate} {
    d->layout = new QHBoxLayout(this);
    d->layout->setContentsMargins({});

    d->loginPage   = new LoginPage(this);
    d->mainPage    = new MainPage(this);
    d->queryPage   = new QueryPage(this);
    d->backendPage = new BackendPage(this);
    d->userPage    = new UserPage(this);

    d->pageMap.insert({SystemPage::Login, d->loginPage});
    d->pageMap.insert({SystemPage::Main, d->mainPage});
    d->pageMap.insert({SystemPage::Query, d->queryPage});
    d->pageMap.insert({SystemPage::Backend, d->backendPage});
    d->pageMap.insert({SystemPage::User, d->userPage});

    for (auto &[_, w] : d->pageMap) { w->hide(); }

    switchTo(SystemPage::Login);

    connect(d->loginPage, &LoginPage::loginPassed, this, [this](QString username) {
        d->currentUser = username;
        d->userPage->updateByUserName(getCurrentUid(), d->currentUser);
        switchToMain();
    });
    connect(d->mainPage, &MainPage::queryRequest, this, &RailwaySystem::query);
    connect(d->mainPage, SIGNAL(shouldGotoBackend()), this, SLOT(gotoBackend()));
    connect(d->mainPage, &MainPage::shouldGotoMine, this, [this] {
        d->userPage->refresh();
        switchTo(SystemPage::User);
        resize(400, 500);
    });
    connect(d->backendPage, SIGNAL(shouldGoBack()), this, SLOT(switchToMain()));
    connect(d->userPage, SIGNAL(shouldGoBack()), this, SLOT(switchToMain()));
    connect(d->queryPage, SIGNAL(shouldGoBack()), this, SLOT(switchToMain()));
    connect(d->queryPage, &QueryPage::purchaseRequest, this, &RailwaySystem::purchase);
    connect(d->userPage, &UserPage::refundRequest, this, &RailwaySystem::refund);
}

RailwaySystem::~RailwaySystem() {
    delete d;
}

void RailwaySystem::switchTo(SystemPage page) {
    assert(d->pageMap.count(page));
    QWidget *nextPage = d->pageMap.at(page);
    assert(nextPage != nullptr);
    if (d->currentPage != nullptr && d->currentPage != nextPage) { d->currentPage->hide(); }
    d->currentPage = nextPage;
    if (d->layout->indexOf(d->currentPage)) { d->layout->addWidget(d->currentPage); }
    d->currentPage->show();
}

void RailwaySystem::switchToMain() {
    switchTo(SystemPage::Main);
    resize(400, 144);
}

void RailwaySystem::query(QString from, QString to, QString date) {
    d->queryPage->updateQueryResults(from, to, date);
    switchTo(SystemPage::Query);
    resize(400, 500);
}

void RailwaySystem::gotoBackend() {
    if (d->currentUser != "admin") {
        PopupMessage::spawn("no access permission", this);
        return;
    }
    d->backendPage->refresh();
    switchTo(SystemPage::Backend);
    resize(800, 400);
}

void RailwaySystem::purchase(QString trainId, int totalTickets) {
    assert(totalTickets > 0);
    auto     &db = DatabaseManager::getInstance();
    QSqlQuery query{};
    auto      sql = QString("select total from view_remain_tickets where train_id = '%1';").arg(trainId);
    auto      ok  = db.execute(query, sql);
    assert(ok);
    ok = query.next();
    assert(ok);
    auto leftTickets = query.value(0).toInt(&ok);
    assert(ok);
    if (totalTickets > leftTickets) {
        PopupMessage::spawn("there're not enough tickets left", this);
        return;
    }
    auto uid = getCurrentUid();
    sql      = QString(R"(
         insert into tickets (train_id, user_id, purchase_time, is_group)
         values ('%1', '%2', '%3', '%4');)")
              .arg(trainId)
              .arg(uid)
              .arg(QDate::currentDate().toString("yyyy-MM-dd"))
              .arg(totalTickets > 1 ? "true" : "false");
    ok = db.execute(query, sql);
    assert(ok);
    if (totalTickets == 1) { return; }
    auto ticketId = query.lastInsertId().toInt(&ok);
    assert(ok);
    sql = QString(R"(
        insert into integrate (ticket_id, group_size)
        values ('%1', '%2');)")
              .arg(ticketId)
              .arg(totalTickets);
    ok = db.execute(query, sql);
    assert(ok);
}

void RailwaySystem::refund(QString ticketId) {
    auto     &db = DatabaseManager::getInstance();
    QSqlQuery query{};
    auto      sql = QString("delete from tickets where ticket_id = '%1';").arg(ticketId);
    auto      ok  = db.execute(query, sql);
    assert(ok);
    d->userPage->refresh();
}

int RailwaySystem::getCurrentUid() {
    assert(!d->currentUser.isEmpty());
    auto     &db = DatabaseManager::getInstance();
    QSqlQuery query{};
    auto      sql = QString("select user_id from account where username = '%1';").arg(d->currentUser);
    auto      ok  = db.execute(query, sql);
    ok            = query.next();
    assert(ok);
    auto uid = query.value(0).toInt(&ok);
    assert(ok);
    return uid;
}
