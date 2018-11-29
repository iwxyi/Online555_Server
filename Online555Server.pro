#-------------------------------------------------
#
# Project created by QtCreator 2018-11-10T12:04:27
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Online555Server
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    globals.cpp \
    fileutil.cpp \
    stringutil.cpp \
    userlist.cpp \
    sessioninfo.cpp

HEADERS  += mainwindow.h \
    globals.h \
    fileutil.h \
    stringutil.h \
    userlist.h \
    sessioninfo.h

FORMS    += mainwindow.ui
