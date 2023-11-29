#include <QSqlQuery>
#include <QFile>
#include <QDir>
#include <QDebug>
#include <memory>

#include "dbmanager.h"

DBManager &DBManager::getInstance() {
    static std::unique_ptr<DBManager> DBM_INSTANCE{new DBManager};
    return *DBM_INSTANCE.get();
}

DBManager::DBManager()
    : db_{QSqlDatabase::addDatabase("QSQLITE")} {
    auto shouldInit = checkOrCreateDatabase();
    db_.setDatabaseName(dbpath());
    auto ok = db_.open();
    Q_ASSERT(ok);
    if (shouldInit) { setupDatabase(); }
}

DBManager::~DBManager() {
    db_.close();
}

QString DBManager::dbpath() const {
    return {"data/railway.db"};
}

bool DBManager::validateOrCreateAccount(const QString &username, const QString &password, bool validateOnly) {
    Q_ASSERT(db_.isOpen());
    QSqlQuery query;
    query.prepare("select count(*) from account where username = ? and password = ?;");
    query.addBindValue(username);
    query.addBindValue(password);
    query.exec();
    auto ok = query.next();
    Q_ASSERT(ok);
    auto totalRecords = query.value(0).toInt();
    Q_ASSERT(totalRecords >= 0 && totalRecords <= 1);
    auto exists = totalRecords > 0;
    if (validateOnly) { return exists; }
    query.prepare("select count(*) from account where username = ?;");
    query.addBindValue(username);
    query.exec();
    ok = query.next();
    Q_ASSERT(ok);
    totalRecords = query.value(0).toInt();
    Q_ASSERT(totalRecords >= 0 && totalRecords <= 1);
    exists = totalRecords > 0;
    if (exists) { return false; }
    query.prepare("insert into account (username, password) values (?, ?);");
    query.addBindValue(username);
    query.addBindValue(password);
    ok = query.exec();
    Q_ASSERT(ok);
    return true;
}

bool DBManager::execSqlSource(const QString &path) {
    auto success = true;
    if (!db_.isOpen()) { return false; }

    QFile sqlFile(path);
    auto  ok = sqlFile.open(QFile::ReadOnly | QFile::Text);
    if (!ok) { return false; }
    QTextStream stream(&sqlFile);
    auto        sql = stream.readAll();
    sqlFile.close();

    ok = db_.transaction();
    if (!ok) { return false; }
    auto      sqlList = sql.split(";", Qt::SkipEmptyParts);
    QSqlQuery query;
    for (const auto &sqlItem : sqlList) {
        auto sql = sqlItem.trimmed();
        if (sql.isEmpty()) { continue; }
        sql.append(';');
        ok = query.exec(sql);
        if (!ok) {
            success = false;
            db_.rollback();
        }
    }
    ok = db_.commit();
    if (!ok) { return false; }

    return success;
}

bool DBManager::checkOrCreateDatabase() const {
    auto path = dbpath();
    QDir().mkpath(QFileInfo(path).absolutePath());
    if (!QFile::exists(path)) {
        QFile db(path);
        auto  ok = db.open(QIODevice::WriteOnly);
        Q_ASSERT(ok);
        db.close();
        return true;
    }
    return false;
}

void DBManager::setupDatabase() {
    auto ok = execSqlSource(":/sql/schema.sql");
    Q_ASSERT(ok);
}
