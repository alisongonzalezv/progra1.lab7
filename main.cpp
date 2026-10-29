#include <QApplication>
#include "Lab7.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    Lab7 window;
    window.show();

    return app.exec();
}