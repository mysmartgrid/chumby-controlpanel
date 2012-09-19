# -*- mode: cmake; -*-

set(clockplugin_SRCS
  alarmdetails.cpp
  alarmform.cpp
  daypage.cpp
  namepage.cpp
  clockplugin.cpp
  clockwidget.cpp
  snoozepage.cpp
  sourcepage.cpp
  timepage.cpp
  volumepage.cpp
  )
set(clockplugin_HDRS
  alarmdetails.h
  alarmform.h
  daypage.h
  namepage.h
  clockplugin.h
  clockwidget.h
  snoozepage.h
  sourcepage.h
  timepage.h
  volumepage.h
  )

set(alarmdetails_UIS alarmdetails.ui)
set(alarmdetails__MOC_HDRS alarmdetails.h)

set(alarmform_UIS alarmform.ui)
set(alarmform__MOC_HDRS alarmform.h)

set(daypage_UIS daypage.ui)
set(daypage__MOC_HDRS daypage.h)

set(namepage_UIS namepage.ui)
set(namepage__MOC_HDRS namepage.h)

set(clockwidget_UIS clockwidget.ui)
set(clockwidget__MOC_HDRS clockwidget.h)

set(snoozepage_UIS snoozepage.ui)
set(snoozepage__MOC_HDRS snoozepage.h)

set(sourcepage_UIS sourcepage.ui)
set(sourcepage__MOC_HDRS sourcepage.h)

set(timepage_UIS timepage.ui)
set(timepage__MOC_HDRS timepage.h)

set(volumepage_UIS volumepage.ui)
set(volumepage__MOC_HDRS volumepage.h)

qt4_add_resources(clockplugin_RC_SRCS ${clockplugin_RESOURCES})
qt4_wrap_ui(clockplugin_UI_HDRS
  ${alarmdetails_UIS}
  ${alarmform_UIS}
  ${daypage_UIS}
  ${namepage_UIS}
  ${clockwidget_UIS}
  ${snoozepage_UIS}
  ${sourcepage_UIS}
  ${timepage_UIS}
  ${volumepage_UIS}
  )
qt4_wrap_cpp(clockplugin_MOC_SRCS
  ${clockplugin_HDRS}
  )

include_directories(${CMAKE_SOURCE_DIR}/controlpanel)
include_directories(${CMAKE_BINARY_DIR}/controlpanel)
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${ASOUND_INCLUDE_DIR})



add_definitions(-DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_CORE_LIB -DQT_SHARED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pipe -g -Wall -W -D_REENTRANT")

add_library(clockplugin MODULE ${clockplugin_SRCS}
  ${clockplugin_MOC_SRCS} ${clockplugin_RC_SRCS}
  ${clockplugin_UI_HDRS}
  )

set_target_properties(clockplugin PROPERTIES SOVERSION "1.0")
set_target_properties(clockplugin PROPERTIES INSTALL_RPATH "/mnt/usb/lib")
set_target_properties(clockplugin PROPERTIES BUILD_WITH_INSTALL_RPATH true)

install(TARGETS clockplugin
  LIBRARY DESTINATION ${PLUGIN_DIR})
