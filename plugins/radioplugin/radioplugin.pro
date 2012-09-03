TARGET = radioplugin
TEMPLATE = lib
CONFIG += dll

QMAKE_CXXFLAGS += -fPIC -shared
QMAKE_LFLAFGS = -ldl

HEADERS += radioplugin.h \
	crad/crad_interface.h \
	crad/crad_internal.h \
	crad/crad_rds_decoder.h \
	crad/qndriver.h \ 
	crad/qnio.h \
    radioplugin.h

SOURCES += radioplugin.cpp \
	crad/crad_return_codes.c \
	crad/crad_interface.c \
	crad/crad_rds_decoder.c \
	crad/qndriver.c \
	crad/qnio.c

DESTDIR = ../

RESOURCES += \
    radioplugin.qrc
