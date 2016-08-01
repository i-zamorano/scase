#-------------------------------------------------
#
# Project created by QtCreator 2014-01-21T21:52:44
#
#-------------------------------------------------


QT       += core widgets

TESTING_STATIC = NO

isEqual(TESTING_STATIC, YES) {
    CONFIG += static
}

CONFIG += x86_64
CONFIG -= x86

CONFIG += c++11
CONFIG += plugin

QMAKE_LFLAGS += -Wall

TARGET = DocumentWriter
TEMPLATE = lib

DEFINES += SCASE1_PLUGIN_DOCUMENTWRITER_LIBRARY

USE_PREDICTOR = YES

isEqual(USE_PREDICTOR, YES) {
    DEFINES += SCASE1_PLUGIN_DOCUMENTWRITER_PREDICTION_ENABLED
}

debug {
    DEFINES += SCASE1_PLUGIN_DEBUG_LEVEL_VERBOSE
}

INCLUDEPATH += ../../core

SOURCES += documentwriterplugin.cpp \
    dwptextedit.cpp

HEADERS += documentwriterplugin.h\
        scase1_plugin_documentwriter_global.h \
    dwptextedit.h \
    dwppresagecallback.h

isEqual(USE_PREDICTOR, YES) {
    win32 {
        PRESAGE_ROOT = ../../windows_lib/presage_091/64
    }
    unix {
        PRESAGE_ROOT = /usr/local/
    }

    PRESAGE_ROOT = $${PRESAGE_ROOT}

    isEmpty(PRESAGE_ROOT) {
        error(Environment variable PRESAGE_ROOT must be set)
    }

    ! exists($${PRESAGE_ROOT}) {
        error(PRESAGE_ROOT does not exist. Please set environment variable PRESAGE_ROOT to a valid presage installation)
    }

    INCLUDEPATH += $${PRESAGE_ROOT}include
    LIBS += -L$${PRESAGE_ROOT}lib -lpresage
}
