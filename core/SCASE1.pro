#-------------------------------------------------
#
# Project created by QtCreator 2014-01-19T21:24:15
#
#-------------------------------------------------

QT       += core gui xml multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

DEFINES += SCASE1_FULLSCREEN

debug {
    DEFINES += SCASE1_DEBUG_LEVEL_VERBOSE
}

TARGET = SCASE1
TEMPLATE = app

CONFIG += x86_64
CONFIG -= x86
CONFIG += c++11

QMAKE_CXXFLAGS += -coverage
QMAKE_LFLAGS += -Wall -fprofile-arcs -ftest-coverage

SOURCES += main.cpp\
        mainwindow.cpp \
    browser.cpp \
    browserlevel.cpp \
    browseritem.cpp \
    pluginhandler.cpp \
    browserpresentationwidget.cpp \
    interactionwidget.cpp

HEADERS  += mainwindow.h \
    interfaces/IBrowserLevel.h \
    interfaces/IBrowserItem.h \
    interfaces/IPlugin.h \
    browser.h \
    browserlevel.h \
    browseritem.h \
    pluginhandler.h \
    interfaces/IBrowserDelegate.h \
    interfaces/IBrowserPresentationDelegate.h \
    browserpresentationwidget.h \
    interactionwidget.h
