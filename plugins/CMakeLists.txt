# -*- mode: cmake; -*-

set(_plugindirs 
        chumbykbddriver
        birdsplugin
        clockplugin
        dispvaluesplugin
        helloworldplugin
        netconfigplugin
        radioplugin
)

message(STATUS "adding plugins.")
foreach(_plugin ${_plugindirs})
  if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${_plugin} )
    if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${_plugin}/CMakeLists.txt )
      message(STATUS "adding plugin ${_plugin}.")
      add_subdirectory(${_plugin})
    else (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${_plugin}/CMakeLists.txt )
      message(STATUS "plugin ${_plugin} is not cmakeproject.")
    endif (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${_plugin}/CMakeLists.txt )
  endif (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${_plugin} )
endforeach()
