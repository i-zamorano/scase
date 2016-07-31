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

PLUGIN_LIB_NAME = DocumentWriter

win32 {
    PLUGIN_BUILD_DIR = "..\\..\\plugins\\$${PLUGIN_LIB_NAME}\\debug"
}
unix {
    PLUGIN_BUILD_DIR = ../../../builds/plugins/$${PLUGIN_LIB_NAME}/debug
}


isEmpty(PLUGIN_BUILD_DIR) {
    error(Environment variable PLUGIN_BUILD_DIR must be set)
}

INCLUDEPATH += ../plugins/$${PLUGIN_LIB_NAME}
#LIBS += -L$${PLUGIN_BUILD_DIR}
LIBS += -L"$${OUT_PWD}\\debug"
LIBS += -l$${PLUGIN_LIB_NAME}

unix {
    PRESAGE_ROOT = /usr/local/
}

win32 {
    PRESAGE_ROOT = "C:\\Program Files\\presage\\"
}

INCLUDEPATH += $${PRESAGE_ROOT}include
LIBS += -L$${PRESAGE_ROOT}lib -lpresage
