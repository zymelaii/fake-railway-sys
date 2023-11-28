#include <QApplication>

#include "railwaysystem.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    RailwaySystem w;
    w.setWindowTitle("Railway System");
    w.show();

    return app.exec();
}
