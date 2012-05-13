#-------------------------------------------------
#
# Project created by QtCreator 2012-05-03T16:51:42
#
#-------------------------------------------------

QT          += core gui
CONFIG      += qwt

TARGET      = ressler_d
TEMPLATE    = app


SOURCES     += main.cpp\
            mainwindow.cpp

HEADERS     += mainwindow.h

FORMS       += mainwindow.ui

LIBS += -L/usr/local/qwt-6.0.1/lib/ -lqwt
INCLUDEPATH += /usr/local/qwt-6.0.1/include
DEPENDPATH += /usr/local/qwt-6.0.1/include
