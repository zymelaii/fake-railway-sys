#pragma once

#include <QWidget>

class RailwaySystemPrivate;

class RailwaySystem : public QWidget {
    Q_OBJECT

public:
    explicit RailwaySystem(QWidget *parent = nullptr);
    ~RailwaySystem();

    enum SystemPage {
        Login,
        Main,
        Query,
        User,
        Backend,
    };

    Q_ENUM(SystemPage);

public slots:
    void switchTo(SystemPage page);
    void switchToMain();
    void query(QString from, QString to, QString date);

protected slots:
    void gotoBackend();
    void purchase(QString trainId, int totalTickets);
    void refund(QString ticketId);

protected:
    int getCurrentUid();

private:
    RailwaySystemPrivate *d;
};
