#pragma once

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTableView>
#include <QHeaderView>

namespace Ui {

class BackendPage {
public:
    QWidget     *containerAdd;
    QWidget     *containerRemove;
    QTableView  *tableView;
    QLineEdit   *inputRecordValue[7];
    QLineEdit   *inputTrainId;
    QPushButton *btAdd;
    QPushButton *btRemove;
    QPushButton *btReturn;
    QVBoxLayout *layout;
    QHBoxLayout *layoutAdd;
    QHBoxLayout *layoutRemove;

    void setupUi(QWidget *backendPage) {
        if (backendPage->objectName().isEmpty()) { backendPage->setObjectName("BackendPage"); }

        //! widget
        containerAdd    = new QWidget(backendPage);
        containerRemove = new QWidget(backendPage);
        tableView       = new QTableView(backendPage);
        for (auto &input : inputRecordValue) { input = new QLineEdit(containerAdd); }
        inputTrainId = new QLineEdit(containerRemove);
        btAdd        = new QPushButton("Add schedule", containerAdd);
        btRemove     = new QPushButton("Remove schedule", containerRemove);
        btReturn     = new QPushButton("back →", containerRemove);

        //! layout
        layout = new QVBoxLayout(backendPage);
        layout->addWidget(tableView);
        layout->addWidget(containerAdd);
        layout->addWidget(containerRemove);

        layoutAdd = new QHBoxLayout(containerAdd);
        for (const auto &input : inputRecordValue) { layoutAdd->addWidget(input); }
        layoutAdd->addWidget(btAdd);

        layoutRemove = new QHBoxLayout(containerRemove);
        layoutRemove->addWidget(inputTrainId);
        layoutRemove->addWidget(btRemove);
        layoutRemove->addWidget(btReturn);

        //! property
        layout->setContentsMargins({});
        layout->setSpacing(2);

        QMargins margin{4, 3, 4, 3};
        layoutAdd->setContentsMargins(margin);
        layoutAdd->setSpacing(8);
        layoutRemove->setContentsMargins(margin);
        layoutRemove->setSpacing(8);

        tableView->verticalHeader()->hide();
        tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

        inputRecordValue[0]->setPlaceholderText("列车号");
        inputRecordValue[1]->setPlaceholderText("出发地");
        inputRecordValue[2]->setPlaceholderText("目的地");
        inputRecordValue[3]->setPlaceholderText("出发时间");
        inputRecordValue[4]->setPlaceholderText("抵达时间");
        inputRecordValue[5]->setPlaceholderText("票价");
        inputRecordValue[6]->setPlaceholderText("总座位数");

        inputTrainId->setPlaceholderText("待删除列车号");
    }
};

} // namespace Ui
