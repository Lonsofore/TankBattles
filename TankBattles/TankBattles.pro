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
    health.cpp \
    button.cpp \
    player.cpp \
    block.cpp \
    textpanel.cpp \
    numupdown.cpp \
    delay.cpp

HEADERS  += \
    tank.h \
    bullet.h \
    enemy.h \
    game.h \
    score.h \
    health.h \
    button.h \
    player.h \
    block.h \
    textpanel.h \
    numupdown.h \
    delay.h

RESOURCES += \
    res.qrc
