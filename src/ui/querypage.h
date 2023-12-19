#pragma once

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QDate>

namespace Ui {

class QueryPage {
public:
    QLabel      *lbDate;
    QLabel      *lbRoute;
    QGroupBox   *resultGroup;
    QVBoxLayout *layout;
    QVBoxLayout *layoutResult;
    QPushButton *btReturn;

    void setupUi(QWidget *queryPage) {
        if (queryPage->objectName().isEmpty()) { queryPage->setObjectName("QueryPage"); }

        //! widget
        lbDate      = new QLabel(queryPage);
        lbRoute     = new QLabel(queryPage);
        resultGroup = new QGroupBox("车票列表", queryPage);
        btReturn    = new QPushButton("返回", queryPage);

        //! layout
        layout = new QVBoxLayout(queryPage);
        layout->addWidget(lbDate);
        layout->addWidget(lbRoute);
        layout->addWidget(resultGroup);
        layout->addWidget(btReturn);

        layoutResult = new QVBoxLayout(resultGroup);

        //! property
        layout->setSpacing(8);

        lbDate->setAlignment(Qt::AlignCenter);
        lbDate->setMaximumHeight(lbDate->fontMetrics().height());

        lbRoute->setAlignment(Qt::AlignCenter);
        lbRoute->setMaximumHeight(lbRoute->fontMetrics().height());
    }
};

} // namespace Ui
