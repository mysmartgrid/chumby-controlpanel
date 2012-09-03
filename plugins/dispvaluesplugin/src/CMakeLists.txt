# -*- mode: cmake; -*-

set(dispvaluesplugin_SRCS
  dispval.cpp
  main.cpp
  json/json.cpp
  plotter/plotsettings.cpp
  plotter/plotter.cpp
  plotter/qpointd.cpp
  spritevis/qcustomgraphview.cpp
  spritevis/tacho.cpp
  spritevis/zeiger.cpp
  )
set(dispvaluesplugin_HDRS
  dispval.h
  json/json.h
  plotter/converter.h
  plotter/plotsettings.h
  plotter/plotter.h
  plotter/qpointd.h
  spritevis/qcustomgraphview.h
  spritevis/tacho.h
  spritevis/zeiger.h
  )

include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/controlpanel)

qt4_add_resources(dispvaluesplugin_RC_SRCS ../dispval.qrc)
qt4_wrap_cpp( dispvaluesplugin_MOC_SRCS
  dispval.h
  spritevis/tacho.h
  spritevis/qcustomgraphview.h
  plotter/plotter.h
)

add_library(dispvaluesplugin MODULE ${dispvaluesplugin_SRCS}
  ${dispvaluesplugin_MOC_SRCS}
  ${dispvaluesplugin_RC_SRCS}
)

set_target_properties(dispvaluesplugin PROPERTIES SOVERSION "1.0")
set_target_properties(dispvaluesplugin PROPERTIES INSTALL_RPATH "/mnt/usb/lib")
set_target_properties(dispvaluesplugin PROPERTIES BUILD_WITH_INSTALL_RPATH true)

install(TARGETS dispvaluesplugin 
  LIBRARY DESTINATION plugins)
