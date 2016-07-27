#-------------------------------------------------
#
# Project created by QtCreator 2014-01-21T21:52:44
#
#-------------------------------------------------


QT       += core widgets

CONFIG += x86_64
CONFIG -= x86
CONFIG += c++11

QMAKE_CXXFLAGS += -coverage
QMAKE_LFLAGS += -Wall -fprofile-arcs -ftest-coverage

TARGET = DocumentWriter
TEMPLATE = lib

DEFINES += SCASE1_PLUGIN_DOCUMENTWRITER_LIBRARY

debug {
    DEFINES += SCASE1_PLUGIN_DEBUG_LEVEL_VERBOSE
}

DEFINES += SCASE1_PLUGIN_DOCUMENTWRITER_PREDICTION_ENABLED

INCLUDEPATH += ../../core

unix {
    INCLUDEPATH += /usr/local/include
    LIBS += -L/usr/local/lib -lpresage
}

SOURCES += documentwriterplugin.cpp \
    dwptextedit.cpp

HEADERS += documentwriterplugin.h\
        scase1_plugin_documentwriter_global.h \
    dwptextedit.h \
    dwppresagecallback.h

dist.path = dist
mac:dist.files = $$OUT_PWD/*DocumentWriter.dylib

scase1.path = ../SCASE1/plugins
mac:scase1.files = $$OUT_PWD/*DocumentWriter.dylib

INSTALLS += dist scase1
