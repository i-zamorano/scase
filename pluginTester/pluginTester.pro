#-------------------------------------------------
#
# Project created by QtCreator 2016-07-29T16:12:15
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = pluginTester
TEMPLATE = app

SOURCES += main.cpp

HEADERS  +=

INCLUDEPATH += ../core

CONFIG += c++11
CONFIG -= release
CONFIG += debug

PLUGIN_LIB_NAME = DocumentWriter

PLUGIN_BUILD_DIR = ../../../builds/plugins/$${PLUGIN_LIB_NAME}/debug

isEmpty(PLUGIN_BUILD_DIR) {
    error(Environment variable PLUGIN_BUILD_DIR must be set)
}

INCLUDEPATH += ../plugins/$${PLUGIN_LIB_NAME}
LIBS += -L$${PLUGIN_BUILD_DIR} -l$${PLUGIN_LIB_NAME}

unix {
    PRESAGE_ROOT = /usr/local/
}

win32 {
    PRESAGE_ROOT = "C:\\Program Files\\presage\\"
}

INCLUDEPATH += $${PRESAGE_ROOT}include
LIBS += -L$${PRESAGE_ROOT}lib -lpresage
