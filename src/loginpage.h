#pragma once

#include <QWidget>

namespace Ui {
class LoginPage;
}; // namespace Ui

class LoginPagePrivate;

class LoginPage : public QWidget {
    Q_OBJECT

public:
    explicit LoginPage(QWidget *parent = nullptr);
    ~LoginPage();

private:
    bool eventFilter(QObject *object, QEvent *e) override;
    void resizeEvent(QResizeEvent *e) override;

signals:
    void loginPassed(QString username);

protected slots:
    void switchToLoginStage();
    void switchToRegisterStage();
    void submitLoginForm();
    void submitRegisterForm();

private:
    LoginPagePrivate *d;
    Ui::LoginPage    *ui;
};
