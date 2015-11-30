#-------------------------------------------------
#
# Project created by QtCreator 2015-11-04T11:38:14
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TankBattles_MapEditor
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    preview.cpp \
    block.cpp

HEADERS  += mainwindow.h \
    preview.h \
    block.h

FORMS    += mainwindow.ui \
    preview.ui

RESOURCES += \
    map_textures.qrc
