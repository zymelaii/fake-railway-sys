#pragma once

#include <QWidget>
#include <QLabel>
#include <QGroupBox>
#include <QPushButton>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>

namespace Ui {

class UserPage {
public:
    QLabel      *lbUserName;
    QGroupBox   *ticketGroup;
    QPushButton *btReturn;
    QVBoxLayout *layout;
    QVBoxLayout *layoutTicket;

    void setupUi(QWidget *userPage) {
        if (userPage->objectName().isEmpty()) { userPage->setObjectName("UserPage"); }

        //! widget
        lbUserName  = new QLabel(userPage);
        ticketGroup = new QGroupBox("车票", userPage);
        btReturn    = new QPushButton("返回", userPage);

        //! layout
        layout = new QVBoxLayout(userPage);
        layout->addWidget(lbUserName);
        layout->addWidget(ticketGroup);
        layout->addWidget(btReturn);

        layoutTicket = new QVBoxLayout(ticketGroup);

        //! property
        layoutTicket->setSpacing(12);

        lbUserName->setAlignment(Qt::AlignCenter);
        lbUserName->setMaximumHeight(lbUserName->fontMetrics().height());
    }
};

} // namespace Ui
