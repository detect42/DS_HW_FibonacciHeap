#include <QApplication>
#include "mywidget.h"

int main(int argc, char *argv[]) {
    srand(114514);
    QApplication app(argc, argv);

    MyWidget widget;
    widget.setWindowTitle("Graphs Printer");

    widget.show();

    return app.exec();
}
