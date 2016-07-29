#-------------------------------------------------
#
# Project created by QtCreator 2016-07-29T16:12:15
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = pluginTester
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

INCLUDEPATH += ../core
INCLUDEPATH += ../plugins/DocumentWriter
LIBS += -L../plugins/build-SCASE1_Plugin_DocumentWriter-Desktop_Qt_5_5_1_clang_64bit-Debug -lDocumentWriter

unix {
    INCLUDEPATH += /usr/local/include
    LIBS += -L/usr/local/lib -lpresage
}

win32 {
    PRESAGE_PATH = C:\Program Files\presage
    INCLUDEPATH += $${PRESAGE_PATH}\include
    LIBS += -L$${PRESAGE_PATH}\lib -lpresage
}
