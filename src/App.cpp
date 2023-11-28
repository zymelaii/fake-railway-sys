#include <QApplication>

#include "RailwaySystem.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    RailwaySystem w;
    w.show();

    return app.exec();
}
