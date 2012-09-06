# -*- mode: cmake; -*-

set(chumbykbddriver_SRCS
  chumbykbddriverplugin.cpp
  chumbykbdhandler.cpp
  )
set(chumbykbddriver_HDRS
chumbykbddriverplugin.h
chumbykbdhandler.h
  )

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/controlpanel)

#qt4_add_resources(chumbykbddriver_RC_SRCS ../dispval.qrc)
qt4_wrap_cpp( chumbykbddriver_MOC_SRCS
  ${chumbykbddriver_HDRS}
)

add_library(chumbykbddriver MODULE ${chumbykbddriver_SRCS}
  ${chumbykbddriver_MOC_SRCS}
  ${chumbykbddriver_RC_SRCS}
)

set_target_properties(chumbykbddriver PROPERTIES SOVERSION "1.0")
set_target_properties(chumbykbddriver PROPERTIES INSTALL_RPATH "/mnt/usb/lib")
set_target_properties(chumbykbddriver PROPERTIES BUILD_WITH_INSTALL_RPATH true)

install(TARGETS chumbykbddriver 
  LIBRARY DESTINATION plugins)
