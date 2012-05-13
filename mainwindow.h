#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_marker.h>
#include <qwt_symbol.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private:
    Ui::MainWindow *ui;
    QwtPlotCurve *curve1, *curve2, *curve3, *curve4, *curve5;
    QwtPlotMarker* mark1, *mark2;

    int j;
    double *x1, *y1, *z1;
    double *x2, *y2, *z2;
    double *dx1dt, *dy1dt, *dz1dt;
    double *dx2dt, *dy2dt, *dz2dt;
    double *dx1dt2, *dy1dt2, *dz1dt2;
    double *dx2dt2, *dy2dt2, *dz2dt2;
    double *u1;  //частота
    double *u2;

    double w1;
    double w2;
    double d;
    double a;
    double r;
    double Tmin;
    double Tmax;
    int stepCount;

    bool solved;
    void solver(double x10, double y10, double z10, double x20, double y20, double z20);

    double dx1(double x1, double y1, double z1, double x2, double y2, double z2)
    {
        return -w1*y1 - z1;
    }


    double dy1(double x1, double y1, double z1, double x2, double y2, double z2)
    {
        if ((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) < r * r)
            return w1 * x1 + a * y1 + d * (y2 - y1);
        else return w1 * x1 + a * y1;
    }

    double dz1(double x1, double y1, double z1, double x2, double y2, double z2)
    {
        return 0.1 + z1 * (x1 - 8.5);
    }

    double dx2(double x1, double y1, double z1, double x2, double y2, double z2)
    {
        return -w2 * y2 - z2;
    }

    double dy2(double x1, double y1, double z1, double x2, double y2, double z2)
    {
        if ((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) < r * r)
            return w2 * x2 + a * y2 + d * (y1 - y2);
        else return w2 * x2 + a * y2;
    }

    double dz2(double x1, double y1, double z1, double x2, double y2, double z2)
    {
        return 0.1 + z2 * (x2 - 8.5);
    }

    void paintGraphs();

protected:
    void timerEvent(QTimerEvent *e);
};

#endif // MAINWINDOW_H
