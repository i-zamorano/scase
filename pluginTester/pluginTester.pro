#-------------------------------------------------
#
# Project created by QtCreator 2016-07-29T16:12:15
#
#-------------------------------------------------

QT       += core

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = pluginTester
TEMPLATE = app

SOURCES += main.cpp

HEADERS  +=

INCLUDEPATH += ../core

CONFIG += x86_64
CONFIG -= x86
CONFIG += c++11

QMAKE_LFLAGS += -Wall

CONFIG -= release
CONFIG += debug

DEFINES += SCASE1_PLUGIN_DOCUMENTWRITER_PREDICTION_ENABLED

PLUGIN_LIB_NAME = DocumentWriter

INCLUDEPATH += ../plugins/$${PLUGIN_LIB_NAME}
win32 {
    LIBS += -L"$${OUT_PWD}\\debug"
}
unix {
    LIBS += -L"$${OUT_PWD}/debug"
}

LIBS += -l$${PLUGIN_LIB_NAME}

unix {
    PRESAGE_ROOT = /usr/local/
}

win32 {
    PRESAGE_ROOT = "C:\\Program Files\\presage\\"
}

INCLUDEPATH += $${PRESAGE_ROOT}include
LIBS += -L$${PRESAGE_ROOT}lib -lpresage
