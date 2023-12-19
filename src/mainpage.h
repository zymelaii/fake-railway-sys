#pragma once

#include <QWidget>

namespace Ui {
class MainPage;
}; // namespace Ui

class MainPagePrivate;

class MainPage : public QWidget {
    Q_OBJECT

public:
    explicit MainPage(QWidget *parent = nullptr);
    ~MainPage();

signals:
    void queryRequest(QString from, QString to, QString date);
    void shouldGotoBackend();
    void shouldGotoMine();

protected slots:
    void swapRoute();
    void submitQueryForm();

private:
    MainPagePrivate *d;
    Ui::MainPage    *ui;
};
