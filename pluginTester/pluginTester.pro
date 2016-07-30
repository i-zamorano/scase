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
INCLUDEPATH += ../plugins/DocumentWriter

CONFIG += c++11

CONFIG(debug) {
    win32 {
        BUILD_MODE = debug
    }
    unix {
        BUILD_MODE = Debug
    }
}
CONFIG(release) {
    win32 {
        BUILD_MODE = release
    }
    unix {
        BUILD_MODE = Release
    }
}

win32 {
    PLUGIN_BUILD_DIR = builds/$${BUILD_MODE}
}
unix {
    PLUGIN_BUILD_DIR = build-SCASE1_Plugin_DocumentWriter-Desktop_Qt_5_5_1_clang_64bit-$${BUILD_MODE}
}

PLUGIN_BUILD_DIR = ../plugins/$${PLUGIN_BUILD_DIR}
PLUGIN_LIB_NAME = DocumentWriter

isEmpty(PLUGIN_BUILD_DIR) {
    error(Environment variable PLUGIN_BUILD_DIR must be set)
}

! exists($${PLUGIN_BUILD_DIR}) {
    error($${PLUGIN_BUILD_DIR})
}

LIBS += -L$${PLUGIN_BUILD_DIR} -l$${PLUGIN_LIB_NAME}

unix {
    PRESAGE_ROOT = /usr/local/
}

win32 {
    PRESAGE_ROOT = "C:\\Program Files\\presage\\"
}

INCLUDEPATH += $${PRESAGE_ROOT}include
