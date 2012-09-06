# -*- mode: cmake; -*-

set(netconfigplugin_SRCS
  main.cpp
  wizard.cpp
  chumbyvkbd/chumbyvkbd.cpp
  chumbyvnumpad/chumbyvnumpad.cpp
  )
set(netconfigplugin_HDRS
  wizard.h
  chumbyvkbd/chumbyvkbd.h
  chumbyvnumpad/chumbyvnumpad.h
  )

qt4_add_resources(netconfigplugin_RC_SRCS 
  chumbyvkbd/icons_kbd.qrc 
  chumbyvnumpad/icons_numblock.qrc 
  icons.qrc
)
qt4_wrap_cpp(netconfigplugin_MOC_SRCS
  ${netconfigplugin_HDRS}
  )

include_directories(${CMAKE_SOURCE_DIR}/controlpanel)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_definitions(-DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_CORE_LIB -DQT_SHARED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pipe -g -Wall -W -D_REENTRANT")

add_library(netconfigplugin MODULE ${netconfigplugin_SRCS}
  ${netconfigplugin_RC_SRCS}
  ${netconfigplugin_MOC_SRCS}
)
target_link_libraries(netconfigplugin ${QT_QTXML_LIBRARY})

set_target_properties(netconfigplugin PROPERTIES SOVERSION "1.0")
set_target_properties(netconfigplugin PROPERTIES INSTALL_RPATH "/mnt/usb/lib")
set_target_properties(netconfigplugin PROPERTIES BUILD_WITH_INSTALL_RPATH true)

install(TARGETS netconfigplugin
  LIBRARY DESTINATION plugins)
