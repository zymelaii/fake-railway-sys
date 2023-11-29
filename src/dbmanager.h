#pragma once

#include <QSqlDatabase>
#include <QSqlResult>

class DBManager {
public:
    static DBManager &getInstance();

protected:
    DBManager();

public:
    ~DBManager();

    QString dbpath() const;
    bool    validateOrCreateAccount(const QString &username, const QString &password, bool validateOnly);
    bool    execSqlSource(const QString &path);

private:
    bool checkOrCreateDatabase() const;
    void setupDatabase();

private:
    QSqlDatabase db_;
};
