#-------------------------------------------------
#
# Project created by QtCreator 2012-01-10T11:01:24
#
#-------------------------------------------------

QT       += core gui

TARGET = chumby-controlpanel
TEMPLATE = app

QMAKE_CXXFLAGS += -fPIC -frtti
QMAKE_LFLAGS += -rdynamic

SOURCES += main.cpp\
        controlpanel.cpp \
    pluginmanager.cpp \
    dllmanager.cpp \
    plugin.cpp

HEADERS  += controlpanel.h \
    pluginmanager.h \
    plugin.h \
    dllmanager.h

LIBS += -ldl
