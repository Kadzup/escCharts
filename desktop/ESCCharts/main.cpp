#include "esccharts.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ESCCharts w;
    w.show();
    return a.exec();
}
