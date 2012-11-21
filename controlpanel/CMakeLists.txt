# -*- mode: cmake; -*-

# export QT_QWS_FONTDIR=/mnt/usb/lib/fonts

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_BINARY_DIR}/generated/config.h)

set(controlpanel_SRCS
  alarm.cpp
  alarmdaemon.cpp
  controlpanel.cpp
  main.cpp
  musiccontrol.cpp
  alarmwidget.cpp
  plugin.cpp
  audioplugin.cpp
  dllmanager.cpp
  #  PlaybackThread.cpp
  )
IF ( BLUETUNE_FOUND )
set(controlpanel_SRCS ${controlpanel_SRCS}
  btwrapper.cpp
  ringtoneplugin.cpp)
ENDIF ( BLUETUNE_FOUND )

set(controlpanel_HDRS
  alarm.h
  alarmdaemon.h
  controlpanel.h			
  musiccontrol.h
  alarmwidget.h
  plugin.h
  audioplugin.h
  #  dllmanager.h
  #  PlaybackThread.h
  )
IF ( BLUETUNE_FOUND )
set(controlpanel_HDRS ${controlpanel_HDRS}
  btwrapper.h
  ringtoneplugin.h)
ENDIF ( BLUETUNE_FOUND )

set(controlpanel_RCS controlpanel.qrc)

#set(loadingWidget_UIS loadingWidget.ui )
#set(loadingWidget_MOC_HDRS loadingWidget.h)

#set(alarmwidget_UIS alarmwidget.ui)
#set(alarmwidget_MOC_HDRS alarmwidget.h)

#set(controlpanel_UIS loadingWidget.ui )
#set(controlpanel_MOC_HDRS controlpanel.h)

#controlpanel.pro
#resources/

qt4_add_resources(controlpanel_RC_SRCS ${controlpanel_RCS})
qt4_wrap_ui (controlpanel_UI_HDRS 
  ${loadingWidget_UIS}
  ${alarmwidget_UIS}
  ${controlpanel_UIS} 
  alarmwidget.ui
  controlpanel.ui
  loadingWidget.ui
  )
qt4_wrap_cpp( controlpanel_MOC_SRCS
  ${controlpanel_HDRS}
  )

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_BINARY_DIR}/generated)
include_directories(${ASOUND_INCLUDE_DIR})

IF ( BLUETUNE_FOUND AND ATOMIX_FOUND AND NEPTUNE_FOUND )
  include_directories(${BLUETUNE_INCLUDE_DIRS})
  include_directories(${ATOMIX_INCLUDE_DIR})
  include_directories(${NEPTUNE_INCLUDE_DIR})
ENDIF ( BLUETUNE_FOUND AND ATOMIX_FOUND AND NEPTUNE_FOUND )

add_executable(chumby-controlpanel  ${controlpanel_SRCS}
  ${controlpanel_MOC_SRCS}
  ${controlpanel_RC_SRCS} 
  ${controlpanel_UI_HDRS}
  )
target_link_libraries(chumby-controlpanel ${QT_QTGUI_LIBRARY})
target_link_libraries(chumby-controlpanel ${QT_QTNETWORK_LIBRARY})
target_link_libraries(chumby-controlpanel ${QT_QTCORE_LIBRARY})
target_link_libraries(chumby-controlpanel ${ASOUND_LIBRARY})
target_link_libraries(chumby-controlpanel iw)
target_link_libraries(chumby-controlpanel dl)
target_link_libraries(chumby-controlpanel rt)
IF ( Q_WS_QWS )
	target_link_libraries(chumby-controlpanel ts)
ENDIF ( Q_WS_QWS )

IF ( BLUETUNE_FOUND AND ATOMIX_FOUND AND NEPTUNE_FOUND )
  target_link_libraries(chumby-controlpanel ${BLUETUNE_LIBRARY})
  target_link_libraries(chumby-controlpanel ${ATOMIX_LIBRARY})
  target_link_libraries(chumby-controlpanel ${NEPTUNE_LIBRARY})
ENDIF ( BLUETUNE_FOUND AND ATOMIX_FOUND AND NEPTUNE_FOUND )

set_target_properties(chumby-controlpanel PROPERTIES INSTALL_RPATH "/mnt/usb/lib")
set_target_properties(chumby-controlpanel PROPERTIES BUILD_WITH_INSTALL_RPATH true)

install(TARGETS chumby-controlpanel
  RUNTIME DESTINATION ${PANEL_DIR})
