# -*- mode: cmake; -*-

set(radioplugin_SRCS
  radioplugin.cpp
  #crad/crad_content_handler.cpp
  #crad/crad_crossdomain_handler.cpp
  crad/crad_interface.c
  crad/crad_rds_decoder.c
  crad/crad_return_codes.c
  crad/qndriver.c
  crad/qnio.c
  )

set(radioplugin_HDRS
  radioplugin.h
  #crad/crad_content_handler.h
  #crad/crad_crossdomain_handler.h
  crad/crad_interface.h
  crad/crad_internal.h
  crad/crad_rds_decoder.h
  crad/qndriver.h
  crad/qnio.h
  )

include_directories(${CMAKE_SOURCE_DIR}/controlpanel)
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${ASOUND_INCLUDE_DIR})


qt4_add_resources(radioplugin_RC_SRCS radioplugin.qrc)
qt4_wrap_cpp( radioplugin_MOC_SRCS
  radioplugin.h
)
add_library(radioplugin MODULE ${radioplugin_SRCS}
  ${radioplugin_RC_SRCS}
  ${radioplugin_MOC_SRCS}
  )

#set_target_properties(radioplugin PROPERTIES SOVERSION "1.0")
set_target_properties(radioplugin PROPERTIES INSTALL_RPATH "/mnt/usb/lib")
set_target_properties(radioplugin PROPERTIES BUILD_WITH_INSTALL_RPATH true)

install(TARGETS radioplugin
  LIBRARY DESTINATION ${PLUGIN_DIR})

