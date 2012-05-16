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

//    enum { num = 2 };
    
private:
    Ui::MainWindow *ui;
    QwtPlotCurve **curve; //*curve [num];
    QwtPlotMarker **mark;

    int t;
    double **x, **y, **z;
    double **dxdt, **dydt, **dzdt;
    double **dxdt2, **dydt2, **dzdt2;
    double **u;  //частота

    double *w;
    double d;
    double a;
    double r;
    double Tmin;
    double Tmax;
    int count;
    int num;

    bool solved;
    void solver(double *x0, double *y0, double *z0);

    double dx(double *X, double *Y, double *Z, int k, int l)
    {
        return -w[k] * Y[k] - Z[k];
    }


    double dy(double *X, double *Y, double *Z, int k, int l)
    {
        double sync = 0;
        for (int i = 0; i < num; ++i)
            if ((x[i][l] - x[k][l]) * (x[i][l] - x[k][l]) + (y[i][l] - y[k][l]) * (y[i][l] - y[k][l]) < r * r)
                sync += d * (Y[i] - Y[k]);

        return w[k] * X[k] + a * Y[k] + sync;
    }

    double dz(double *X, double *Y, double *Z, int k, int l)
    {
        return 0.1 + Z[k] * (X[k] - 8.5);
    }

//    void paintGraphs();

protected:
    void timerEvent(QTimerEvent *e);
};

#endif // MAINWINDOW_H
