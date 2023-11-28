#pragma once

#include <QDialog>

class PopupMessage : public QDialog {
public:
    static void spawn(const QString &msg, QWidget *parent);

protected:
    explicit PopupMessage(const QString &msg, QWidget *parent);
};
