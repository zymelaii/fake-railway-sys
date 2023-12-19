#pragma once

#include <QSqlDatabase>
#include <QSqlResult>
#include <optional>

class DatabaseManager {
public:
    static DatabaseManager &getInstance();

protected:
    DatabaseManager();
    DatabaseManager(const DatabaseManager &)            = delete;
    DatabaseManager(DatabaseManager &&)                 = delete;
    DatabaseManager &operator=(const DatabaseManager &) = delete;

public:
    ~DatabaseManager();

    QString dbpath() const;
    bool    validateOrCreateAccount(const QString &username, const QString &password, bool validateOnly);
    bool    execSqlSource(const QString &path);
    bool    execute(QSqlQuery &query, const QString &sql);

private:
    bool checkOrCreateDatabase() const;
    void setupDatabase();

private:
    QSqlDatabase db_;
};
