#pragma once

#include <QWidget>

namespace Ui {
class QueryPage;
}; // namespace Ui

class QueryPagePrivate;

class QueryPage : public QWidget {
    Q_OBJECT

public:
    explicit QueryPage(QWidget *parent = nullptr);
    ~QueryPage();

signals:
    void shouldGoBack();
    void purchaseRequest(QString trainId, int totalTickets);

public slots:
    void updateQueryResults(const QString &from, const QString &to, const QString &date);

protected:
    struct ScheduleInfo {
        QString trainId;
        QString departureTime;
        QString arrivalTime;
        QString ticketPrice;
    };

    void clearResults();
    void addResult(const ScheduleInfo &schedule);

private:
    QueryPagePrivate *d;
    Ui::QueryPage    *ui;
};
