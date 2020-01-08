#include "esccharts.h"
#include "ui_esccharts.h"

ESCCharts::ESCCharts(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ESCCharts)
{
    ui->setupUi(this);
}

ESCCharts::~ESCCharts()
{
    delete ui;
}

