#pragma once

#include <QWidget>

namespace Ui {
class BackendPage;
}; // namespace Ui

class BackendPagePrivate;

class BackendPage : public QWidget {
    Q_OBJECT

public:
    explicit BackendPage(QWidget *parent = nullptr);
    ~BackendPage();

signals:
    void shouldGoBack();

public slots:
    void refresh();

protected slots:
    void tryAddScheduleRecord();
    void tryRemoveScheduleRecord();

protected:
    bool isTrainIdExists(const QString &id);
    bool isTrainIdValid(const QString &id);
    bool isDateValid(const QString &date);

private:
    BackendPagePrivate *d;
    Ui::BackendPage    *ui;
};
