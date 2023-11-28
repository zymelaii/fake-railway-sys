#pragma once

#include <QWidget>

namespace Ui {
class RailwaySystem;
}; // namespace Ui

class RailwaySystemPrivate;

class RailwaySystem : public QWidget {
    Q_OBJECT

public:
    explicit RailwaySystem(QWidget *parent = nullptr);
    ~RailwaySystem();

private:
    bool eventFilter(QObject *object, QEvent *e) override;
    void resizeEvent(QResizeEvent *e) override;

protected slots:
    void switchToLoginStage();
    void switchToRegisterStage();
    void submitLoginForm();
    void submitRegisterForm();

private:
    RailwaySystemPrivate *d;
    Ui::RailwaySystem    *ui;
};
