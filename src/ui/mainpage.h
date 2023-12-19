#pragma once

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDate>

namespace Ui {

class MainPage {
public:
    QWidget     *inputContainer;
    QWidget     *buttonContainer;
    QLineEdit   *inputSrc;
    QLineEdit   *inputDst;
    QPushButton *btSwap;
    QPushButton *btQuery;
    QPushButton *btBackend;
    QPushButton *btMine;
    QLineEdit   *inputDate;
    QVBoxLayout *layout;
    QHBoxLayout *layoutSel;
    QGridLayout *layoutButton;

    void setupUi(QWidget *mainPage) {
        if (mainPage->objectName().isEmpty()) { mainPage->setObjectName("MainPage"); }

        //! widget
        inputContainer  = new QWidget(mainPage);
        buttonContainer = new QWidget(mainPage);
        inputSrc        = new QLineEdit(inputContainer);
        inputDst        = new QLineEdit(inputContainer);
        btSwap          = new QPushButton("↔", inputContainer);
        btQuery         = new QPushButton("query", mainPage);
        btBackend       = new QPushButton("backend", buttonContainer);
        btMine          = new QPushButton("mine", buttonContainer);
        inputDate       = new QLineEdit(mainPage);

        //! layout
        layout = new QVBoxLayout(mainPage);
        layout->addWidget(inputContainer);
        layout->addWidget(inputDate);
        layout->addWidget(btQuery);
        layout->addWidget(buttonContainer);

        layoutSel = new QHBoxLayout(inputContainer);
        layoutSel->addWidget(inputSrc);
        layoutSel->addWidget(btSwap);
        layoutSel->addWidget(inputDst);

        layoutButton = new QGridLayout(buttonContainer);
        layoutButton->addWidget(btBackend, 0, 0);
        layoutButton->addWidget(btMine, 0, 1);

        //! property
        btSwap->setObjectName("SwapButton");

        btQuery->setObjectName("QueryButton");

        layoutSel->setContentsMargins({});
        layoutButton->setContentsMargins({});

        inputSrc->setAlignment(Qt::AlignCenter);
        inputSrc->setPlaceholderText("departure");

        inputDst->setAlignment(Qt::AlignCenter);
        inputDst->setPlaceholderText("destination");

        inputDate->setText(QDate::currentDate().toString("yyyy/MM/dd"));
        inputDate->setAlignment(Qt::AlignCenter);
        inputDate->setPlaceholderText("departure date");
    }
};

} // namespace Ui
