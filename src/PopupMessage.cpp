#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QTimer>

#include "PopupMessage.h"

void PopupMessage::spawn(const QString &msg, QWidget *parent) {
    auto popup = new PopupMessage(msg, parent);
    popup->show();
}

PopupMessage::PopupMessage(const QString &msg, QWidget *parent)
    : QDialog(parent) {
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_TransparentForMouseEvents);

    setFixedSize(parent->size());
    move(0, 20);

    setStyleSheet(R"(
        #MessagePane {
            background-color: rgb(60, 60, 60);
            border-radius: 4px;
        }
        QLabel {
            font: 12px;
            color: azure;
        }
    )");

    QMargins margins(10, 6, 10, 6);

    auto pane = new QWidget(this);
    pane->setObjectName("MessagePane");
    pane->setMaximumWidth(width() / 2);

    auto label    = new QLabel(msg, pane);
    auto maxWidth = pane->maximumWidth() - (margins.left() + margins.right());
    label->adjustSize();
    label->setAlignment(Qt::AlignCenter);
    label->setFixedWidth(qMin(label->width(), maxWidth));
    label->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
    label->setWordWrap(true);

    auto layoutPane = new QGridLayout(pane);
    layoutPane->addWidget(label);
    layoutPane->setContentsMargins(margins);

    auto layout = new QVBoxLayout(this);
    layout->addWidget(pane);
    layout->addStretch();
    layout->unsetContentsMargins();
    layout->setAlignment(Qt::AlignCenter);

    auto opacityEffect = new QGraphicsOpacityEffect(this);
    opacityEffect->setOpacity(0.8);
    setGraphicsEffect(opacityEffect);

    auto opacityAnimation = new QPropertyAnimation(opacityEffect, "opacity");
    opacityAnimation->setDuration(500);
    opacityAnimation->setStartValue(opacityEffect->opacity());
    opacityAnimation->setEndValue(0.0);

    auto positionAnimation = new QPropertyAnimation(this, "pos");
    positionAnimation->setDuration(500);
    positionAnimation->setStartValue(pos());
    positionAnimation->setEndValue(pos() - QPoint(0, label->height()));

    QTimer::singleShot(1000, this, [=]() {
        opacityAnimation->start();
        positionAnimation->start();
        connect(opacityAnimation, SIGNAL(finished()), this, SLOT(close()));
    });
}
