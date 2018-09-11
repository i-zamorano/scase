#-------------------------------------------------
#
# Project created by QtCreator 2014-01-21T21:52:44
#
#-------------------------------------------------


QT       += core widgets texttospeech

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

TARGET = FileManager
TEMPLATE = lib

DEFINES += SCASE1_PLUGIN_FILEMANAGER_LIBRARY

debug {
    DEFINES += SCASE1_PLUGIN_DEBUG_LEVEL_VERBOSE
}

INCLUDEPATH += ../../core

SOURCES += filemanagerplugin.cpp

HEADERS += filemanagerplugin.h\
        scase1_plugin_filemanager_global.h

