#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    curve1 = new QwtPlotCurve("Curve 1");
    curve1->setRenderHint(QwtPlotItem::RenderAntialiased);
    curve2 = new QwtPlotCurve("Curve 2");
    curve2->setRenderHint(QwtPlotItem::RenderAntialiased);
    curve3 = new QwtPlotCurve("Curve 3");
    curve3->setRenderHint(QwtPlotItem::RenderAntialiased);

    curve4 = new QwtPlotCurve("Curve 4");
    curve4->setRenderHint(QwtPlotItem::RenderAntialiased);
    curve4->setPen(QPen(Qt::blue, 1));
    curve5 = new QwtPlotCurve("Curve 5");
    curve5->setRenderHint(QwtPlotItem::RenderAntialiased);
    curve5->setPen(QPen(Qt::red,1));

    mark1 = new QwtPlotMarker;
    mark1->setSymbol(new QwtSymbol(QwtSymbol::Ellipse, QColor(Qt::transparent), QColor(Qt::blue), QSize(5,5)));
    mark2 = new QwtPlotMarker;
    mark2->setSymbol(new QwtSymbol(QwtSymbol::Ellipse, QColor(Qt::transparent), QColor(Qt::red), QSize(5,5)));

    ui->qwtPlot->setAxisScale(QwtPlot::xBottom, -20, 20);
    ui->qwtPlot->setAxisScale(QwtPlot::yLeft, -20, 20);
    ui->qwtPlot_2->setAxisScale(QwtPlot::xBottom, -20, 20);
    ui->qwtPlot_2->setAxisScale(QwtPlot::yLeft, -20, 20);
    ui->qwtPlot_4->setAxisScale(QwtPlot::xBottom, -20, 20);
    ui->qwtPlot_4->setAxisScale(QwtPlot::yLeft, -20, 20);

    solved = false;
    w1 = 0.98;
    w2 = 1.02;
    d = 0.5;
    a = 0.16;
    r = 2;
    Tmin = 0;
    Tmax = 200;
    stepCount = 10000;
    j = 0;

    solver(5, 5, 5, 1, 1, 1);
    paintGraphs();
    startTimer(1);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintGraphs()
{
    if ((!solved))
        return;

    curve1->setRawSamples(x1, y1, stepCount);
    curve1->attach(ui->qwtPlot);
    ui->qwtPlot->replot();

    curve2->setRawSamples(x2, y2, stepCount);
    curve2->attach(ui->qwtPlot_2);
    ui->qwtPlot_2->replot();

    double  *i_Tmax = new double [stepCount];
    for (int i = 0; i < stepCount; ++i)
        i_Tmax[i] = i * Tmax / stepCount;
    curve3->setRawSamples(i_Tmax, u1, stepCount);
    curve3->attach(ui->qwtPlot_3);
    ui->qwtPlot_3->replot();

}

void MainWindow::solver(double x10, double y10, double z10, double x20, double y20, double z20)
{
    x1 = new double[stepCount];
    y1 = new double[stepCount];
    z1 = new double[stepCount];
    x2 = new double[stepCount];
    y2 = new double[stepCount];
    z2 = new double[stepCount];
    dx1dt = new double[stepCount];
    dy1dt = new double[stepCount];
    dz1dt = new double[stepCount];
    dx2dt = new double[stepCount];
    dy2dt = new double[stepCount];
    dz2dt = new double[stepCount];

    dx1dt2 = new double[stepCount];
    dy1dt2 = new double[stepCount];
    dx2dt2 = new double[stepCount];
    dy2dt2 = new double[stepCount];

    u1 = new double[stepCount];
    u2 = new double[stepCount];

    x1[0] = x10; y1[0] = y10; z1[0] = z10; u1[0] = 0;
    x2[0] = x20; y2[0] = y20; z2[0] = z20; u2[0] = 0;
    dx1dt[0] = dx1(x1[0], y1[0], z1[0], x2[0], y2[0], z2[0]);
    dy1dt[0] = dy1(x1[0], y1[0], z1[0], x2[0], y2[0], z2[0]);
    dz1dt[0] = dz1(x1[0], y1[0], z1[0], x2[0], y2[0], z2[0]);
    dx2dt[0] = dx2(x2[0], y2[0], z2[0], x2[0], y2[0], z2[0]);
    dy2dt[0] = dy2(x2[0], y2[0], z2[0], x2[0], y2[0], z2[0]);
    dz2dt[0] = dz2(x2[0], y2[0], z2[0], x2[0], y2[0], z2[0]);

    double dt = (Tmax - Tmin) / stepCount;
    int k = 0;

    for (int i = 1; i < stepCount; i++)
    {
        double X1, X2, X3, X4, Y1, Y2, Y3, Y4, Z1, Z2, Z3, Z4;
        double X12, X22, X32, X42, Y12, Y22, Y32, Y42, Z12, Z22, Z32, Z42;

        X1 = dx1(x1[i - 1], y1[i - 1], z1[i - 1], x2[i - 1], y2[i - 1], z2[i - 1]) * dt;
        Y1 = dy1(x1[i - 1], y1[i - 1], z1[i - 1], x2[i - 1], y2[i - 1], z2[i - 1]) * dt;
        Z1 = dz1(x1[i - 1], y1[i - 1], z1[i - 1], x2[i - 1], y2[i - 1], z2[i - 1]) * dt;

        X12 = dx2(x1[i - 1], y1[i - 1], z1[i - 1], x2[i - 1], y2[i - 1], z2[i - 1]) * dt;
        Y12 = dy2(x1[i - 1], y1[i - 1], z1[i - 1], x2[i - 1], y2[i - 1], z2[i - 1]) * dt;
        Z12 = dz2(x1[i - 1], y1[i - 1], z1[i - 1], x2[i - 1], y2[i - 1], z2[i - 1]) * dt;

        X2 = dx1(x1[i - 1] + X1 / 2, y1[i - 1] + Y1 / 2, z1[i - 1] + Z1 / 2,
                 x2[i - 1] + X12 / 2, y2[i - 1] + Y12 / 2, z2[i - 1] + Z12 / 2) * dt;
        Y2 = dy1(x1[i - 1] + X1 / 2, y1[i - 1] + Y1 / 2, z1[i - 1] + Z1 / 2,
                 x2[i - 1] + X12 / 2, y2[i - 1] + Y12 / 2, z2[i - 1] + Z12 / 2) * dt;
        Z2 = dz1(x1[i - 1] + X1 / 2, y1[i - 1] + Y1 / 2, z1[i - 1] + Z1 / 2,
                 x2[i - 1] + X12 / 2, y2[i - 1] + Y12 / 2, z2[i - 1] + Z12 / 2) * dt;

        X22 = dx2(x1[i - 1] + X1 / 2, y1[i - 1] + Y1 / 2, z1[i - 1] + Z1 / 2,
                  x2[i - 1] + X12 / 2, y2[i - 1] + Y12 / 2, z2[i - 1] + Z12 / 2) * dt;
        Y22 = dy2(x1[i - 1] + X1 / 2, y1[i - 1] + Y1 / 2, z1[i - 1] + Z1 / 2,
                  x2[i - 1] + X12 / 2, y2[i - 1] + Y12 / 2, z2[i - 1] + Z12 / 2) * dt;
        Z22 = dz2(x1[i - 1] + X1 / 2, y1[i - 1] + Y1 / 2, z1[i - 1] + Z1 / 2,
                  x2[i - 1] + X12 / 2, y2[i - 1] + Y12 / 2, z2[i - 1] + Z12 / 2) * dt;

        X3 = dx1(x1[i - 1] + X2 / 2, y1[i - 1] + Y2 / 2, z1[i - 1] + Z2 / 2,
                 x2[i - 1] + X22 / 2, y2[i - 1] + Y22 / 2, z2[i - 1] + Z22 / 2) * dt;
        Y3 = dy1(x1[i - 1] + X2 / 2, y1[i - 1] + Y2 / 2, z1[i - 1] + Z2 / 2,
                 x2[i - 1] + X22 / 2, y2[i - 1] + Y22 / 2, z2[i - 1] + Z22 / 2) * dt;
        Z3 = dz1(x1[i - 1] + X2 / 2, y1[i - 1] + Y2 / 2, z1[i - 1] + Z2 / 2,
                 x2[i - 1] + X22 / 2, y2[i - 1] + Y22 / 2, z2[i - 1] + Z22 / 2) * dt;

        X32 = dx2(x1[i - 1] + X2 / 2, y1[i - 1] + Y2 / 2, z1[i - 1] + Z2 / 2,
                  x2[i - 1] + X22 / 2, y2[i - 1] + Y22 / 2, z2[i - 1] + Z22 / 2) * dt;
        Y32 = dy2(x1[i - 1] + X2 / 2, y1[i - 1] + Y2 / 2, z1[i - 1] + Z2 / 2,
                  x2[i - 1] + X22 / 2, y2[i - 1] + Y22 / 2, z2[i - 1] + Z22 / 2) * dt;
        Z32 = dz2(x1[i - 1] + X2 / 2, y1[i - 1] + Y2 / 2, z1[i - 1] + Z2 / 2,
                  x2[i - 1] + X22 / 2, y2[i - 1] + Y22 / 2, z2[i - 1] + Z22 / 2) * dt;

        X4 = dx1(x1[i - 1] + X3, y1[i - 1] + Y3, z1[i - 1] + Z3,
                 x2[i - 1] + X32, y2[i - 1] + Y32, z2[i - 1] + Z32) * dt;
        Y4 = dy1(x1[i - 1] + X3, y1[i - 1] + Y3, z1[i - 1] + Z32,
                 x2[i - 1] + X32, y2[i - 1] + Y32, z2[i - 1] + Z32) * dt;
        Z4 = dz1(x1[i - 1] + X3, y1[i - 1] + Y32, z1[i - 1] + Z32,
                 x2[i - 1] + X32, y2[i - 1] + Y32, z2[i - 1] + Z32) * dt;

        X42 = dx2(x1[i - 1] + X3, y1[i - 1] + Y3, z1[i - 1] + Z3,
                  x2[i - 1] + X32, y2[i - 1] + Y32, z2[i - 1] + Z32) * dt;
        Y42 = dy2(x1[i - 1] + X3, y1[i - 1] + Y3, z1[i - 1] + Z32,
                  x2[i - 1] + X32, y2[i - 1] + Y32, z2[i - 1] + Z32) * dt;
        Z42 = dz2(x1[i - 1] + X3, y1[i - 1] + Y32, z1[i - 1] + Z32,
                  x2[i - 1] + X32, y2[i - 1] + Y32, z2[i - 1] + Z32) * dt;


        x1[i] = x1[i - 1] + (X1 + 2 * X2 + 2 * X3 + X4) / 6;
        y1[i] = y1[i - 1] + (Y1 + 2 * Y2 + 2 * Y3 + Y4) / 6;
        z1[i] = z1[i - 1] + (Z1 + 2 * Z2 + 2 * Z3 + Z4) / 6;

        x2[i] = x2[i - 1] + (X12 + 2 * X22 + 2 * X32 + X42) / 6;
        y2[i] = y2[i - 1] + (Y12 + 2 * Y22 + 2 * Y32 + Y42) / 6;
        z2[i] = z2[i - 1] + (Z12 + 2 * Z22 + 2 * Z32 + Z42) / 6;

        dx1dt[i] = dx1(x1[i], y1[i], z1[i], x2[i], y2[i], z2[i]);
        dy1dt[i] = dy1(x1[i], y1[i], z1[i], x2[i], y2[i], z2[i]);
        dz1dt[i] = dz1(x1[i], y1[i], z1[i], x2[i], y2[i], z2[i]);

        dx2dt[i] = dx2(x1[i], y1[i], z1[i], x2[i], y2[i], z2[i]);
        dy2dt[i] = dy2(x1[i], y1[i], z1[i], x2[i], y2[i], z2[i]);
        dz2dt[i] = dz2(x1[i], y1[i], z1[i], x2[i], y2[i], z2[i]);

        if (i < stepCount && i > 1)
        {
            dx1dt2[i] = (4 * dx1dt[i - 1] - 3 * dx1dt[i - 2] - dx1dt[i]) / (2 * dt);
            dx2dt2[i] = (4 * dx2dt[i - 1] - 3 * dx2dt[i - 2] - dx2dt[i]) / (2 * dt);
            dy1dt2[i] = (4 * dy1dt[i - 1] - 3 * dy1dt[i - 2] - dy1dt[i]) / (2 * dt);
            dy2dt2[i] = (4 * dy2dt[i - 1] - 3 * dy2dt[i - 2] - dy2dt[i]) / (2 * dt);
        }
    }

    solved = true;
    double sum1 = 0, sum2 = 0;
    for (int i = 2; i < stepCount; i++)
    {
        u1[i] = -(dy1dt[i] * dx1dt2[i] - dx1dt[i] * dy1dt2[i]) / (dy1dt[i] * dy1dt[i] + dx1dt[i] * dx1dt[i]);
        u2[i] = -(dy2dt[i] * dx2dt2[i] - dx2dt[i] * dy2dt2[i]) / (dy2dt[i] * dy2dt[i] + dx2dt[i] * dx2dt[i]);
        sum1 += u1[i];
        sum2 += u2[i];
    }

    sum1 /= stepCount - 2;
    sum2 /= stepCount - 2;
}

void MainWindow::timerEvent(QTimerEvent *)
{
    if ((!solved) or (j > stepCount - 2))
        return;
    j++;

    QVector <double> x1r, y1r, x2r, y2r;
    int ind = std::max(0, j - 50000 / (int)Tmax);
    for (int l = ind; l < j; l++)
    {
        x1r.append(x1[l]);
        y1r.append(y1[l]);
        x2r.append(x2[l]);
        y2r.append(y2[l]);
    }
    qDebug()<<j;

    curve4->setSamples(x1r, y1r);
    curve4->attach(ui->qwtPlot_4);
    curve5->setSamples(x2r, y2r);
    curve5->attach(ui->qwtPlot_4);

    mark1->setValue(x1[j],y1[j]);
    mark1->attach(ui->qwtPlot_4);
    mark2->setValue(x2[j],y2[j]);
    mark2->attach(ui->qwtPlot_4);

    ui->qwtPlot_4->replot();
}
