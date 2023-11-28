#pragma once

#include <QDialog>
#include <QLabel>

class PopupMessage : public QDialog {
    Q_OBJECT

public:
    static void spawn(const QString &msg, QWidget *parent);

protected:
    explicit PopupMessage(const QString &msg, QWidget *parent);
};
