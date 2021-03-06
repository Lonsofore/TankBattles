#-------------------------------------------------
#
# Project created by QtCreator 2015-11-05T17:43:47
#
#-------------------------------------------------

QT       += core gui \
        multimedia \
        multimediawidgets \
        network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TankBattles
TEMPLATE = app
RC_FILE = TankBattles.rc


SOURCES += main.cpp \
    tank.cpp \
    bullet.cpp \
    game.cpp \
    score.cpp \
    health.cpp \
    button.cpp \
    player.cpp \
    block.cpp \
    textpanel.cpp \
    numupdown.cpp \
    delay.cpp \
    caution.cpp \
    textbox.cpp \
    bot.cpp

HEADERS  += \
    tank.h \
    bullet.h \
    game.h \
    score.h \
    health.h \
    button.h \
    player.h \
    block.h \
    textpanel.h \
    numupdown.h \
    delay.h \
    caution.h \
    textbox.h \
    bot.h

RESOURCES += \
    res.qrc
