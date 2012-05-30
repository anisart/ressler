#include "mainwindow.h"
#include "ui_mainwindow.h"
#define D_VAL   0.5
#define MU_VAL  0.15

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    num = 7;
    curve = new QwtPlotCurve * [num];
    mark = new QwtPlotMarker * [num];

    for (int j = 0; j < num; ++j)
    {
        curve[j] = new QwtPlotCurve;
        curve[j]->setRenderHint(QwtPlotItem::RenderAntialiased);

        mark[j] = new QwtPlotMarker;
        mark[j]->setSymbol(new QwtSymbol(QwtSymbol::Ellipse, QColor(Qt::transparent), QColor(Qt::black), QSize(5,5)));
    }

    ui->qwtPlot->setAxisScale(QwtPlot::xBottom, -20, 20);
    ui->qwtPlot->setAxisScale(QwtPlot::yLeft, -20, 20);
    grid = new QwtPlotGrid;
    grid->setMajPen(QPen(Qt::black,0,Qt::DotLine));
    grid->attach(ui->qwtPlot);

    d = 0;//0.5;
    a = 0.16;
    mu = 0;//0.15;
    r = 3;
    Tmin = 0;
    Tmax = 200;
    count = 100000;
    t = 0;
    connect(this, SIGNAL(stepChanged(int)), ui->lcdNumber, SLOT(display(int)));
    ui->lcdNumber->setNumDigits(trunc(log10(count)));

    w = new double[num];
    w[0] = 0.95;
    w[1] = 0.97;
    w[2] = 0.99;
    w[3] = 1.01;
    w[4] = 1.03;
    w[5] = 1.05;
    w[num-1] = 1.05;  // not use with etalon

    if (E_NUM)
    {
        curve[num-1]->setPen(QPen(Qt::red,1));
        mark[num-1]->setSymbol(new QwtSymbol(QwtSymbol::Ellipse, QColor(Qt::transparent), QColor(Qt::red), QSize(5,5)));
    }

    x = new double * [num];
    y = new double * [num];
    z = new double * [num];

    for (int j = 0; j < num; ++j)
    {
        x[j] = new double[count];
        y[j] = new double[count];
        z[j] = new double[count];

        x[j][0] = -8 + rand()%17;
        y[j][0] = -8 + rand()%17;
        z[j][0] = -8 + rand()%17;
    }

    if (E_NUM)
    {
        x[num-1][0] = 1;
        y[num-1][0] = 1;
        z[num-1][0] = 0;
    }

    dt = (Tmax - Tmin) / 10000;  // /count;

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(replot()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::solveStep(int i)
{
    double X1[num], X2[num], X3[num], X4[num];
    double Y1[num], Y2[num], Y3[num], Y4[num];
    double Z1[num], Z2[num], Z3[num], Z4[num];

    double X1_[num], X2_[num], X3_[num];
    double Y1_[num], Y2_[num], Y3_[num];
    double Z1_[num], Z2_[num], Z3_[num];

    double x_[num], y_[num], z_[num];
    for (int j = 0; j < num; ++j)
    {
        x_[j] = x[j][i-1];
        y_[j] = y[j][i-1];
        z_[j] = z[j][i-1];
    }

    for (int j = 0; j < num; ++j)
    {
        X1[j] = dx(x_, y_, z_, j, i) * dt;
        Y1[j] = dy(x_, y_, z_, j, i) * dt;
        Z1[j] = dz(x_, y_, z_, j, i) * dt;

        X1_[j] = x[j][i-1] + X1[j] / 2;
        Y1_[j] = y[j][i-1] + Y1[j] / 2;
        Z1_[j] = z[j][i-1] + Z1[j] / 2;
    }

    for (int j = 0; j < num; ++j)
    {
        X2[j] =dx(X1_, Y1_, Z1_, j, i) * dt;
        Y2[j] =dy(X1_, Y1_, Z1_, j, i) * dt;
        Z2[j] =dz(X1_, Y1_, Z1_, j, i) * dt;

        X2_[j] = x[j][i-1] + X2[j] / 2;
        Y2_[j] = y[j][i-1] + Y2[j] / 2;
        Z2_[j] = z[j][i-1] + Z2[j] / 2;
    }

    for (int j = 0; j < num; ++j)
    {
        X3[j] = dx(X2_, Y2_, Z2_, j, i) * dt;
        Y3[j] = dy(X2_, Y2_, Z2_, j, i) * dt;
        Z3[j] = dz(X2_, Y2_, Z2_, j, i) * dt;

        X3_[j] = x[j][i-1] + X3[j];
        Y3_[j] = y[j][i-1] + Y3[j];
        Z3_[j] = z[j][i-1] + Z3[j];
    }

    for (int j = 0; j < num; ++j)
    {
        X4[j] = dx(X3_, Y3_, Z3_, j, i) * dt;
        Y4[j] = dy(X3_, Y3_, Z3_, j, i) * dt;
        Z4[j] = dz(X3_, Y3_, Z3_, j, i) * dt;

        x[j][i] = x[j][i-1] + (X1[j] + 2 * X2[j] + 2 * X3[j] + X4[j]) / 6;
        y[j][i] = y[j][i-1] + (Y1[j] + 2 * Y2[j] + 2 * Y3[j] + Y4[j]) / 6;
        z[j][i] = z[j][i-1] + (Z1[j] + 2 * Z2[j] + 2 * Z3[j] + Z4[j]) / 6;
    }
}

void MainWindow::replot()
{
    emit stepChanged(t);
    t++;
    solveStep(t);

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
        mark[j]->setValue(x[j][t-1], y[j][t-1]);
        mark[j]->attach(ui->qwtPlot);
    }
    ui->qwtPlot->replot();

    if (t > count - 2)
    {
        killTimer(timer->timerId());
        ui->pauseBtn->setEnabled(false);
        return;
    }
}

void MainWindow::on_pauseBtn_clicked()
{
    if (ui->pauseBtn->text() == "Pause")
    {
        timer->stop();
        ui->pauseBtn->setText("Play");
    }
    else
    {
        timer->start(1);
        ui->pauseBtn->setText("Pause");
    }
}

void MainWindow::on_syncBox_clicked()
{
    if (ui->syncBox->isChecked())
        d = 0.5;
    else
        d = 0;
}

void MainWindow::on_muBtn_clicked()
{
    mu = 0.15;
    ui->muBtn->setEnabled(false);
    ui->muBtn->setText("mu = 0.15");
}
