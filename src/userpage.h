#pragma once

#include <QWidget>

namespace Ui {
class UserPage;
}; // namespace Ui

class UserPagePrivate;

class UserPage : public QWidget {
    Q_OBJECT

public:
    explicit UserPage(QWidget *parent = nullptr);
    ~UserPage();

    void updateByUserName(int userId, const QString &username);

public slots:
    void refresh();

signals:
    void shouldGoBack();
    void refundRequest(QString ticketId);

protected:
    struct TicketInfo {
        QString ticketId;
        QString trainId;
        QString departureTime;
        QString number;
    };

    void addTicketInfo(const TicketInfo &ticket);

private:
    UserPagePrivate *d;
    Ui::UserPage    *ui;
};
