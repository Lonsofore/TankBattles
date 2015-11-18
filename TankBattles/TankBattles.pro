#-------------------------------------------------
#
# Project created by QtCreator 2015-11-05T17:43:47
#
#-------------------------------------------------

QT       += core gui \
        multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TankBattles
TEMPLATE = app


SOURCES += main.cpp \
    tank.cpp \
    bullet.cpp \
    enemy.cpp \
    game.cpp \
    score.cpp \
    health.cpp

HEADERS  += \
    tank.h \
    bullet.h \
    enemy.h \
    game.h \
    score.h \
    health.h

RESOURCES += \
    res.qrc
