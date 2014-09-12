#-------------------------------------------------
#
# Project created by QtCreator 2014-06-28T11:41:50
#
#-------------------------------------------------

QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Scoreboard
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    boardw.cpp

HEADERS  += mainwindow.h \
    boardw.h

FORMS    += mainwindow.ui \
    boardw.ui

RESOURCES += \
    resources.qrc
