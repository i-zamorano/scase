#-------------------------------------------------
#
# Project created by QtCreator 2014-01-21T21:52:44
#
#-------------------------------------------------


QT       += core widgets

CONFIG += x86_64
CONFIG -= x86

TARGET = DocumentWriter
TEMPLATE = lib

DEFINES += SCASE1_PLUGIN_DOCUMENTWRITER_LIBRARY

debug {
    DEFINES += SCASE1_PLUGIN_DEBUG_LEVEL_VERBOSE
}

INCLUDEPATH += ../SCASE1

SOURCES += documentwriterplugin.cpp \
    dwptextedit.cpp

HEADERS += documentwriterplugin.h\
        scase1_plugin_documentwriter_global.h \
    dwptextedit.h

dist.path = dist
mac:dist.files = $$OUT_PWD/*DocumentWriter.dylib

scase1.path = ../SCASE1/plugins
mac:scase1.files = $$OUT_PWD/*DocumentWriter.dylib

INSTALLS += dist scase1
