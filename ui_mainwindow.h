/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Sun May 13 15:51:16 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QTabWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "qwt_plot.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_5;
    QTabWidget *tabWidget;
    QWidget *tab;
    QVBoxLayout *verticalLayout;
    QwtPlot *qwtPlot;
    QWidget *tab_2;
    QVBoxLayout *verticalLayout_2;
    QwtPlot *qwtPlot_2;
    QWidget *tab_3;
    QVBoxLayout *verticalLayout_3;
    QwtPlot *qwtPlot_3;
    QWidget *tab_4;
    QVBoxLayout *verticalLayout_4;
    QwtPlot *qwtPlot_4;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(485, 327);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        verticalLayout_5 = new QVBoxLayout(centralWidget);
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(tabWidget->sizePolicy().hasHeightForWidth());
        tabWidget->setSizePolicy(sizePolicy);
        tabWidget->setMinimumSize(QSize(0, 0));
        tabWidget->setMaximumSize(QSize(16777215, 16777215));
        tabWidget->setAutoFillBackground(false);
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        verticalLayout = new QVBoxLayout(tab);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        qwtPlot = new QwtPlot(tab);
        qwtPlot->setObjectName(QString::fromUtf8("qwtPlot"));

        verticalLayout->addWidget(qwtPlot);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        verticalLayout_2 = new QVBoxLayout(tab_2);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        qwtPlot_2 = new QwtPlot(tab_2);
        qwtPlot_2->setObjectName(QString::fromUtf8("qwtPlot_2"));

        verticalLayout_2->addWidget(qwtPlot_2);

        tabWidget->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        verticalLayout_3 = new QVBoxLayout(tab_3);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        qwtPlot_3 = new QwtPlot(tab_3);
        qwtPlot_3->setObjectName(QString::fromUtf8("qwtPlot_3"));

        verticalLayout_3->addWidget(qwtPlot_3);

        tabWidget->addTab(tab_3, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName(QString::fromUtf8("tab_4"));
        verticalLayout_4 = new QVBoxLayout(tab_4);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        qwtPlot_4 = new QwtPlot(tab_4);
        qwtPlot_4->setObjectName(QString::fromUtf8("qwtPlot_4"));

        verticalLayout_4->addWidget(qwtPlot_4);

        tabWidget->addTab(tab_4, QString());

        verticalLayout_5->addWidget(tabWidget);

        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(3);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("MainWindow", "Tab 1", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("MainWindow", "Tab 2", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("MainWindow", "Tab 3", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_4), QApplication::translate("MainWindow", "Tab 4", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
