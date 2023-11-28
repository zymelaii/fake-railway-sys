#pragma once

#include <QWidget>

struct RailwaySystemPrivate;

class RailwaySystem : public QWidget {
    Q_OBJECT

public:
    explicit RailwaySystem(QWidget *parent = nullptr);
    ~RailwaySystem();

public:
    enum class Page {
        Login,
        Register,
    };

    void setPage(Page page);

protected slots:
    void postRegisterForm();

protected:
    bool eventFilter(QObject *object, QEvent *e) override;

private:
    RailwaySystemPrivate *d;
};
