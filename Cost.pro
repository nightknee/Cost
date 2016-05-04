#-------------------------------------------------
#
# Project created by QtCreator 2015-10-05T20:15:47
#
#-------------------------------------------------

QT       += core gui
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Cost
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    addbox.cpp \
    sqlmanager.cpp \
    exceptions/baseexception.cpp \
    exceptions/setexistproductexception.cpp

HEADERS  += mainwindow.h \
    addbox.h \
    sqlmanager.h \
    exceptions/baseexception.h \
    exceptions/setexistproductexception.h

FORMS    += mainwindow.ui \
    dialog.ui
