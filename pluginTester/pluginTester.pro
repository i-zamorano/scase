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

USE_PREDICTOR = YES

isEqual(USE_PREDICTOR, YES) {
    DEFINES += SCASE1_PLUGIN_DOCUMENTWRITER_PREDICTION_ENABLED
}

PLUGIN_LIB_NAME = DocumentWriter

INCLUDEPATH += ../plugins/$${PLUGIN_LIB_NAME}
win32 {
    LIBS += -L"$${OUT_PWD}\\debug"
}
unix {
    LIBS += -L"$${OUT_PWD}"
}

LIBS += -l$${PLUGIN_LIB_NAME}

isEqual(USE_PREDICTOR, YES) {
    win32 {
        INCLUDEPATH += ../../windows_lib/presage_091/32/include
        LIBS += -L../../windows_lib/presage_091/32/lib -lpresage
    }
    unix {
        INCLUDEPATH += /usr/local/inlude
        LIBS += -L/usr/local/lib -lpresage
    }
}
