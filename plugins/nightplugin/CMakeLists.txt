# -*- mode: cmake; -*-

set(nightplugin_SRCS
  alarmdetails.cpp
  alarmform.cpp
  alarmwizard.cpp
  daypage.cpp
  namepage.cpp
  nightplugin.cpp
  nightwidget.cpp
  snoozepage.cpp
  sourcepage.cpp
  summarypage.cpp
  timepage.cpp
  )
set(nightplugin_HDRS
  alarmdetails.h
  alarmform.h
  alarmwizard.h
  daypage.h
  namepage.h
  nightplugin.h
  nightwidget.h
  snoozepage.h
  sourcepage.h
  summarypage.h
  timepage.h
  )

set(alarmdetails_UIS alarmdetails.ui)
set(alarmdetails__MOC_HDRS alarmdetails.h)

set(alarmform_UIS alarmform.ui)
set(alarmform__MOC_HDRS alarmform.h)

set(daypage_UIS daypage.ui)
set(daypage__MOC_HDRS daypage.h)

set(namepage_UIS namepage.ui)
set(namepage__MOC_HDRS namepage.h)

set(nightwidget_UIS nightwidget.ui)
set(nightwidget__MOC_HDRS nightwidget.h)

set(snoozepage_UIS snoozepage.ui)
set(snoozepage__MOC_HDRS snoozepage.h)

set(sourcepage_UIS sourcepage.ui)
set(sourcepage__MOC_HDRS sourcepage.h)

set(summarypage_UIS summarypage.ui)
set(summarypage__MOC_HDRS summarypage.h)

set(timepage_UIS timepage.ui)
set(timepage__MOC_HDRS timepage.h)

qt4_add_resources(nightplugin_RC_SRCS ${nightplugin_RESOURCES})
qt4_wrap_ui(nightplugin_UI_HDRS
  ${alarmdetails_UIS}
  ${alarmform_UIS}
  ${daypage_UIS}
  ${namepage_UIS}
  ${nightwidget_UIS}
  ${snoozepage_UIS}
  ${sourcepage_UIS}
  ${summarypage_UIS}
  ${timepage_UIS}
  )
qt4_wrap_cpp(nightplugin_MOC_SRCS
  ${nightplugin_HDRS}
  )

include_directories(${CMAKE_SOURCE_DIR}/controlpanel)
include_directories(${CMAKE_BINARY_DIR}/controlpanel)
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${ASOUND_INCLUDE_DIR})



add_definitions(-DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_CORE_LIB -DQT_SHARED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pipe -g -Wall -W -D_REENTRANT")

add_library(nightplugin MODULE ${nightplugin_SRCS}
  ${nightplugin_MOC_SRCS} ${nightplugin_RC_SRCS}
  ${nightplugin_UI_HDRS}
  )

#set_target_properties(nightplugin PROPERTIES SOVERSION "1.0")
set_target_properties(nightplugin PROPERTIES INSTALL_RPATH "/mnt/usb/lib")
set_target_properties(nightplugin PROPERTIES BUILD_WITH_INSTALL_RPATH true)

install(TARGETS nightplugin
  LIBRARY DESTINATION ${PLUGIN_DIR})
