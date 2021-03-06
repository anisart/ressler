#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_marker.h>
#include <qwt_symbol.h>
#include <qwt_plot_grid.h>

#define E_NUM 1

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
    QwtPlotCurve **curve;
    QwtPlotMarker **mark;
    QTimer *timer;
    QwtPlotGrid *grid;

    double **x, **y, **z;
    double *w;
    double d, a, mu, r;
    double Tmin, Tmax, dt;
    int count, num;

    int t;
    void solveStep(int i);

    double dx(double *X, double *Y, double *Z, int k, int l)
    {
        if (k == num - E_NUM)
            return -Y[k];
        else
            return -w[k] * Y[k] - Z[k];
    }


    double dy(double *X, double *Y, double *Z, int k, int l)
    {
        if (k == num - E_NUM)
            return X[k] - mu * (1 - X[k] * X[k]) * Y[k];
        else
        {
            double sync = 0;
            for (int i = 0; i < num; ++i)
                if ((x[i][l-1] - x[k][l-1]) * (x[i][l-1] - x[k][l-1])
                        + (y[i][l-1] - y[k][l-1]) * (y[i][l-1] - y[k][l-1]) < r * r)
                    sync += d * (Y[i] - Y[k]);

            return w[k] * X[k] + a * Y[k] + sync;
        }
    }

    double dz(double *X, double *Y, double *Z, int k, int l)
    {
        if (k == num - E_NUM)
            return 0;
        else
            return 0.1 + Z[k] * (X[k] - 8.5);
    }

public slots:
    void replot();

signals:
    void stepChanged(int value);
private slots:
    void on_pauseBtn_clicked();
    void on_syncBox_clicked();
    void on_muBtn_clicked();
};

#endif // MAINWINDOW_H
