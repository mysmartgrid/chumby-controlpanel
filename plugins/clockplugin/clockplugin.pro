TARGET = clockplugin
TEMPLATE = lib
CONFIG += dll

HEADERS += \
	clockplugin.h \
	clockwidget.h \
	alarmform.h \
	alarmwizard.h \
	timepage.h \
	daypage.h \
	summarypage.h \
	sourcepage.h \
	namepage.h \
	snoozepage.h \
	alarmdetails.h

SOURCES += \
	clockplugin.cpp \
	clockwidget.cpp \
	alarmform.cpp \
	alarmwizard.cpp \
	timepage.cpp \
	daypage.cpp \
	summarypage.cpp \
	sourcepage.cpp \
	namepage.cpp \
	snoozepage.cpp \
	alarmdetails.cpp

DESTDIR = ../

FORMS += \
	clockwidget.ui \
	alarmform.ui \
	timepage.ui \
	daypage.ui \
	summarypage.ui \
	sourcepage.ui \
	namepage.ui \
	snoozepage.ui \
	alarmdetails.ui \
	../../controlpanel/alarmwidget.ui
