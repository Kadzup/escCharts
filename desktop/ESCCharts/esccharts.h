#ifndef ESCCHARTS_H
#define ESCCHARTS_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class ESCCharts; }
QT_END_NAMESPACE

class ESCCharts : public QMainWindow
{
    Q_OBJECT

public:
    ESCCharts(QWidget *parent = nullptr);
    ~ESCCharts();

private:
    Ui::ESCCharts *ui;
};
#endif // ESCCHARTS_H
