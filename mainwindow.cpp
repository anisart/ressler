#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    num = 2;
    curve = new QwtPlotCurve * [num];
    mark = new QwtPlotMarker * [num];

    for (int i = 0; i < num; ++i)
    {
        curve[i] = new QwtPlotCurve("Curve "/* + QString(i)*/);
        curve[i]->setRenderHint(QwtPlotItem::RenderAntialiased);
        curve[i]->setPen(QPen(Qt::red,1));

        mark[i] = new QwtPlotMarker;
        mark[i]->setSymbol(new QwtSymbol(QwtSymbol::Ellipse, QColor(Qt::transparent), QColor(Qt::red), QSize(5,5)));
    }

    //    ui->qwtPlot->setAxisScale(QwtPlot::xBottom, -20, 20);
    //    ui->qwtPlot->setAxisScale(QwtPlot::yLeft, -20, 20);

    solved = false;
    d = 0.5;
    a = 0.16;
    r = 2;
    Tmin = 0;
    Tmax = 200;
    count = 10000;
    t = 0;

    w = new double[num]; // !!!
    w[0] = 0.98;
    w[1] = 1.02;

    double startX[] = {5, 1};
    double startY[] = {5, 1};
    double startZ[] = {5, 1};

    solver(startX, startY, startZ);
    //    paintGraphs();
    startTimer(1000);
    //    QwtPlotCurve *cur = new QwtPlotCurve;
    //    curve[0]->setSamples(x[0], y[0], count);
    //    curve[0]->attach(ui->qwtPlot);
    //    ui->qwtPlot->replot();
}

MainWindow::~MainWindow()
{
    delete ui;
}

//void MainWindow::paintGraphs()
//{
//    if ((!solved))
//        return;

//    curve1->setRawSamples(x1, y1, count);
//    curve1->attach(ui->qwtPlot);
//    ui->qwtPlot->replot();

//    curve2->setRawSamples(x2, y2, count);
//    curve2->attach(ui->qwtPlot_2);
//    ui->qwtPlot_2->replot();

//    double  *i_Tmax = new double [count];
//    for (int i = 0; i < count; ++i)
//        i_Tmax[i] = i * Tmax / count;
//    curve3->setRawSamples(i_Tmax, u1, count);
//    curve3->attach(ui->qwtPlot_3);
//    ui->qwtPlot_3->replot();

//}

void MainWindow::solver(double *x0, double *y0, double *z0)
{
    x = new double * [num];
    y = new double * [num];
    z = new double * [num];

    //    dxdt = new double * [num];
    //    dydt = new double * [num];
    //    dzdt = new double * [num];

    //    dxdt2 = new double * [num];
    //    dydt2 = new double * [num];

    //    u = new double * [num];

    //    double X0[num], Y0[num], Z0[num];

    for (int j = 0; j < num; ++j)
    {
        x[j] = new double[count];
        y[j] = new double[count];
        z[j] = new double[count];

        //        dxdt[j] = new double[count];
        //        dydt[j] = new double[count];
        //        dzdt[j] = new double[count];

        //        dxdt2[j] = new double[count];
        //        dydt2[j] = new double[count];

        //        u[j] = new double[count];

        x[j][0] = x0[j]; y[j][0] = y0[j]; z[j][0] = z0[j];
        //        u[j][0] = 0;
        //        X0[i] = x0[i]; Y0[i] = y0[i]; Z0 [i] = z0[i];
    }
    //    for (int i = 0; i < num; ++i)
    //    {
    //        dxdt[i][0] = dx(X0, Y0, Z0, i, 0);
    //        dydt[i][0] = dy(X0, Y0, Z0, i, 0);
    //        dzdt[i][0] = dz(X0, Y0, Z0, i, 0);
    //    }

    double dt = (Tmax - Tmin) / count;

    for (int i = 1; i < count; i++)
    {
        double X1[num], X2[num], X3[num], X4[num];
        double Y1[num], Y2[num], Y3[num], Y4[num];
        double Z1[num], Z2[num], Z3[num], Z4[num];

        double x_[num], y_[num], z_[num];
        for (int j = 0; j < num; ++j)
        {
            x_[j] = x[j][i-1];
            y_[j] = y[j][i-1];
            z_[j] = z[j][i-1];
        }

        for (int j = 0; j < num; ++j)
        {
            X1[j] = x[j][i-1] + dx(x_, y_, z_, j, i) * dt / 2;
            Y1[j] = y[j][i-1] + dy(x_, y_, z_, j, i) * dt / 2;
            Z1[j] = z[j][i-1] + dz(x_, y_, z_, j, i) * dt / 2;
        }

        for (int j = 0; j < num; ++j)
        {
            X2[j] = x[j][i-1] + dx(X1, Y1, Z1, j, i) * dt / 2;
            Y2[j] = y[j][i-1] + dy(X1, Y1, Z1, j, i) * dt / 2;
            Z2[j] = z[j][i-1] + dz(X1, Y1, Z1, j, i) * dt / 2;
        }

        for (int j = 0; j < num; ++j)
        {
            X3[j] = x[j][i-1] + dx(X2, Y2, Z2, j, i) * dt;
            Y3[j] = y[j][i-1] + dy(X2, Y2, Z2, j, i) * dt;
            Z3[j] = z[j][i-1] + dz(X2, Y2, Z2, j, i) * dt;
        }

        for (int j = 0; j < num; ++j)
        {
            X4[j] = dx(X3, Y3, Z3, j, i) * dt;
            Y4[j] = dy(X3, Y3, Z3, j, i) * dt;
            Z4[j] = dz(X3, Y3, Z3, j, i) * dt;

            x[j][i] = x[j][i-1] + (X1[j] + 2 * X2[j] + 2 * X3[j] + X4[j]) / 6;
            y[j][i] = y[j][i-1] + (Y1[j] + 2 * Y2[j] + 2 * Y3[j] + Y4[j]) / 6;
            z[j][i] = z[j][i-1] + (Z1[j] + 2 * Z2[j] + 2 * Z3[j] + Z4[j]) / 6;
        }



        //        dx1dt[i] = dx1(x1[i], y1[i], z1[i], x2[i], y2[i], z2[i]);
        //        dy1dt[i] = dy1(x1[i], y1[i], z1[i], x2[i], y2[i], z2[i]);
        //        dz1dt[i] = dz1(x1[i], y1[i], z1[i], x2[i], y2[i], z2[i]);

        //        if (i < count && i > 1)
        //        {
        //            dx1dt2[i] = (4 * dx1dt[i - 1] - 3 * dx1dt[i - 2] - dx1dt[i]) / (2 * dt);
        //            dx2dt2[i] = (4 * dx2dt[i - 1] - 3 * dx2dt[i - 2] - dx2dt[i]) / (2 * dt);
        //            dy1dt2[i] = (4 * dy1dt[i - 1] - 3 * dy1dt[i - 2] - dy1dt[i]) / (2 * dt);
        //            dy2dt2[i] = (4 * dy2dt[i - 1] - 3 * dy2dt[i - 2] - dy2dt[i]) / (2 * dt);
        //        }
    }

    solved = true;
    //    double sum1 = 0;
    //    for (int i = 2; i < count; i++)
    //    {
    //        u1[i] = -(dy1dt[i] * dx1dt2[i] - dx1dt[i] * dy1dt2[i]) / (dy1dt[i] * dy1dt[i] + dx1dt[i] * dx1dt[i]);
    //        sum1 += u1[i];
    //        sum2 += u2[i];
    //    }

    //    sum1 /= count - 2;
}

void MainWindow::timerEvent(QTimerEvent *e)
{
    if (!solved)
        return;
    t++;

    for (int j = 0; j < num; ++j)
    {
        QVector <double> xr, yr;
        int ind = std::max(0, t - 50000 / (int)Tmax);
        for (int l = ind; l < t; l++)
        {
            xr.append(x[j][l]);
            yr.append(y[j][l]);
        }

        curve[j]->setSamples(xr, yr);
        curve[j]->attach(ui->qwtPlot);
        mark[j]->setValue(x[j][t], y[j][t]);
        mark[j]->attach(ui->qwtPlot);
    }

    ui->qwtPlot->replot();
    if (t > count - 3)
    {
        killTimer(e->timerId());
        return;
    }
}
