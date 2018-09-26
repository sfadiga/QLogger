QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  tst_qloggertest.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../qloggerlib/release/ -lqloggerlib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../qloggerlib/debug/ -lqloggerlib
else:unix: LIBS += -L$$OUT_PWD/../qloggerlib/ -lqloggerlib

INCLUDEPATH += $$PWD/../qloggerlib
DEPENDPATH += $$PWD/../qloggerlib
