#-------------------------------------------------
#
# Project created by QtCreator 2016-11-27T07:00:10
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LittleTimer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    simpletimer.cpp

HEADERS  += mainwindow.h \
    simpletimer.h

FORMS    += mainwindow.ui

#QMAKE_CXXFLAGS +=

QMAKE_LFLAGS += -static
CONFIG += static

CONFIG += c++11
