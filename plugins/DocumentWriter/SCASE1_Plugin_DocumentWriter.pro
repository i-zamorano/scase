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

QMAKE_MAC_SDK = MacOSX

QMAKE_LFLAGS += -Wall
QMAKE_CFLAGS += -g -O0

TARGET = DocumentWriter
TEMPLATE = lib

DEFINES += SCASE1_PLUGIN_DOCUMENTWRITER_LIBRARY

USE_PREDICTOR = NO

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
        INCLUDEPATH += ../../windows_lib/presage_091/32/include
        LIBS += -L../../../windows_lib/presage_091/32/lib -lpresage
    }
    unix {
        INCLUDEPATH += /usr/local/include
        LIBS += -L/usr/local/lib -lpresage
    }
}
