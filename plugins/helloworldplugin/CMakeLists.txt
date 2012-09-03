# -*- mode: cmake; -*-

set(helloworld_SRCS
  helloworld.cpp
  )
set(helloworld_HDRS
  helloworld.h			
  )

include_directories(${CMAKE_SOURCE_DIR}/controlpanel)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_definitions(-DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_CORE_LIB -DQT_SHARED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pipe -g -Wall -W -D_REENTRANT")

add_library(helloworld MODULE ${helloworld_SRCS}
 )
set_target_properties(helloworld PROPERTIES SOVERSION "1.0")
set_target_properties(helloworld PROPERTIES INSTALL_RPATH "/mnt/usb/lib")
set_target_properties(helloworld PROPERTIES BUILD_WITH_INSTALL_RPATH true)

install(TARGETS helloworld
  LIBRARY DESTINATION plugins)
