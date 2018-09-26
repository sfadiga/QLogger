#-------------------------------------------------
#
# Project created by QtCreator 2018-09-15T18:06:30
#
#-------------------------------------------------

QT       -= gui

TARGET = qloggerlib
TEMPLATE = lib
CONFIG += staticlib

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += xmloutput.cpp \
    textoutput.cpp \
    qlogger.cpp \
    configuration.cpp \
    consoleoutput.cpp \
    output.cpp \
    signaloutput.cpp \
    jsonoutput.cpp

HEADERS += \
    xmloutput.h \
    textoutput.h \
    qlogger.h \
    output.h \
    configuration.h \
    qloggerlib_global.h \
    consoleoutput.h \
    signaloutput.h \
    jsonoutput.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
