#-------------------------------------------------
#
# Project created by QtCreator 2013-03-07T16:20:12
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = QLogger
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    xmloutput.cpp \
    textoutput.cpp \
    qlogger.cpp \
    configuration.cpp

HEADERS += \
    xmloutput.h \
    textoutput.h \
    qlogger.h \
    output.h \
    configuration.h \
    configfilehandler.h
