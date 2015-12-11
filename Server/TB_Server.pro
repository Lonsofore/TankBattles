QT += core\
      network
QT -= gui

TARGET = TB_Server
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += \
    server.cpp \
    main.cpp

HEADERS += \
    server.h

