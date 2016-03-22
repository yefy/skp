#include "skp_mainwindow_test.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SkpMainwindowTest w;
    w.show();

    return a.exec();
}

