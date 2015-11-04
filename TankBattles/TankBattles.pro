#-------------------------------------------------
#
# Project created by QtCreator 2015-10-30T18:18:58
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TankBattles
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    game.cpp \
    panzer.cpp \
    tank.cpp

HEADERS  += mainwindow.h \
    game.h \
    panzer.h \
    tank.h

FORMS    += mainwindow.ui \
    game.ui
