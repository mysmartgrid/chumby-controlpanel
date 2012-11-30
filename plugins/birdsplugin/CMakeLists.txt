# -*- mode: cmake; -*-

set(birds_SRCS
  birds.cpp
  birdswidget.cpp
  normalanimation.cpp
  )
set(birds_HDRS
  birds.h
  birdswidget.h
  birdsanimation.h
  normalanimation.h
  )

set(birds_UIS birdswidget.ui)
set(birds__MOC_HDRS birdswidget.h)

set(birds_RESOURCES birds.qrc)

qt4_add_resources(birdsplugin_RC_SRCS ${birds_RESOURCES})
qt4_wrap_ui(birdsplugin_UI_HDRS
  ${birds_UIS}
  )
qt4_wrap_cpp(birdsplugin_MOC_SRCS
  ${birds_HDRS}
  )

include_directories(${CMAKE_SOURCE_DIR}/controlpanel)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_definitions(-DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_CORE_LIB -DQT_SHARED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pipe -g -Wall -W -D_REENTRANT")

add_library(birds MODULE ${birds_SRCS}
  ${birdsplugin_MOC_SRCS} ${birdsplugin_RC_SRCS}
  ${birdsplugin_UI_HDRS}
  )

#set_target_properties(birds PROPERTIES SOVERSION "1.0")
set_target_properties(birds PROPERTIES INSTALL_RPATH "/mnt/usb/lib")
set_target_properties(birds PROPERTIES BUILD_WITH_INSTALL_RPATH true)

install(TARGETS birds
  LIBRARY DESTINATION ${PLUGIN_DIR})
