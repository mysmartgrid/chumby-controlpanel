#-------------------------------------------------
#
# Project created by QtCreator 2012-01-10T11:01:24
#
#-------------------------------------------------

TARGET = chumby-controlpanel
TEMPLATE = app

QMAKE_LFLAGS += -rdynamic
QMAKE_CXXFLAGS += -fPIC

SOURCES += main.cpp\
    controlpanel.cpp \
    dllmanager.cpp \
    plugin.cpp \
    musiccontrol.cpp \
    alarmdaemon.cpp \
    audioplugin.cpp \
    alarmwidget.cpp

HEADERS  += controlpanel.h \
    plugin.h \
    dllmanager.h \
    musiccontrol.h \
    alarmdaemon.h \
    audioplugin.h \
    alarmwidget.h

LIBS += -ldl -liw -lasound

RESOURCES += \
    controlpanel.qrc

FORMS += \
    controlpanel.ui \
    loadingWidget.ui \
    alarmwidget.ui
