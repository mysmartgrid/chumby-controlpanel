# -*- mode: cmake; -*-
# - Try to find liblibts include dirs and libraries
# Usage of this module as follows:
# This file defines:
# * LIBTS_FOUND if protoc was found
# * LIBTS_LIBRARY The lib to link to (currently only a static unix lib, not
# portable)
# * LIBTS_INCLUDE The include directories for liblibts.

message(STATUS "FindLibts check")
IF (NOT WIN32)
  include(FindPkgConfig)
  if ( PKG_CONFIG_FOUND )

     pkg_check_modules (PC_LIBTS tslib>=1.0)

     set(LIBTS_DEFINITIONS ${PC_LIBTS_CFLAGS_OTHER})
  endif(PKG_CONFIG_FOUND)
endif (NOT WIN32)

#
# set defaults
SET(_libts_HOME "/usr/local")
SET(_libts_INCLUDE_SEARCH_DIRS
  ${CMAKE_INCLUDE_PATH}
#  /usr/local/include
#  /usr/include
  )

SET(_libts_LIBRARIES_SEARCH_DIRS
  ${CMAKE_LIBRARY_PATH}
#  /usr/local/lib
#  /usr/lib
  )

##
if( "${LIBTS_HOME}" STREQUAL "")
  if("" MATCHES "$ENV{LIBTS_HOME}")
    message(STATUS "LIBTS_HOME env is not set, setting it to /usr/local")
    set (LIBTS_HOME ${_libts_HOME})
  else("" MATCHES "$ENV{LIBTS_HOME}")
    set (LIBTS_HOME "$ENV{LIBTS_HOME}")
  endif("" MATCHES "$ENV{LIBTS_HOME}")
else( "${LIBTS_HOME}" STREQUAL "")
  message(STATUS "LIBTS_HOME is not empty: \"${LIBTS_HOME}\"")
endif( "${LIBTS_HOME}" STREQUAL "")
##

message(STATUS "Looking for libts in ${LIBTS_HOME}")

IF( NOT ${LIBTS_HOME} STREQUAL "" )
    SET(_libts_INCLUDE_SEARCH_DIRS ${LIBTS_HOME}/include ${_libts_INCLUDE_SEARCH_DIRS})
    SET(_libts_LIBRARIES_SEARCH_DIRS ${LIBTS_HOME}/lib ${_libts_LIBRARIES_SEARCH_DIRS})
    SET(_libts_HOME ${LIBTS_HOME})
ENDIF( NOT ${LIBTS_HOME} STREQUAL "" )

IF( NOT $ENV{LIBTS_INCLUDEDIR} STREQUAL "" )
  SET(_libts_INCLUDE_SEARCH_DIRS $ENV{LIBTS_INCLUDEDIR} ${_libts_INCLUDE_SEARCH_DIRS})
ENDIF( NOT $ENV{LIBTS_INCLUDEDIR} STREQUAL "" )

IF( NOT $ENV{LIBTS_LIBRARYDIR} STREQUAL "" )
  SET(_libts_LIBRARIES_SEARCH_DIRS $ENV{LIBTS_LIBRARYDIR} ${_libts_LIBRARIES_SEARCH_DIRS})
ENDIF( NOT $ENV{LIBTS_LIBRARYDIR} STREQUAL "" )

IF( LIBTS_HOME )
  SET(_libts_INCLUDE_SEARCH_DIRS ${LIBTS_HOME}/include ${_libts_INCLUDE_SEARCH_DIRS})
  SET(_libts_LIBRARIES_SEARCH_DIRS ${LIBTS_HOME}/lib ${_libts_LIBRARIES_SEARCH_DIRS})
  SET(_libts_HOME ${LIBTS_HOME})
ENDIF( LIBTS_HOME )

# find the include files
FIND_PATH(LIBTS_INCLUDE_DIR tslib.h
   HINTS
     ${_libts_INCLUDE_SEARCH_DIRS}
     ${PC_LIBTS_INCLUDEDIR}
     ${PC_LIBTS_INCLUDE_DIRS}
#    ${CMAKE_INCLUDE_PATH}
)

# locate the library
IF(WIN32)
  SET(LIBTS_LIBRARY_NAMES ${LIBTS_LIBRARY_NAMES} libts.lib)
ELSE(WIN32)
  SET(LIBTS_LIBRARY_NAMES ${LIBTS_LIBRARY_NAMES} libts.a libts.so)
ENDIF(WIN32)
FIND_LIBRARY(LIBTS_LIBRARY NAMES ${LIBTS_LIBRARY_NAMES}
  HINTS
    ${_libts_LIBRARIES_SEARCH_DIRS}
    ${PC_LIBTS_LIBDIR}
    ${PC_LIBTS_LIBRARY_DIRS}
)

get_filename_component(LIBTS_LIBRARY_DIR "${LIBTS_LIBRARY}" PATH)
set(LIBTS_PLUGIN_DIR "${LIBTS_LIBRARY_DIR}/ts")

# if the include and the program are found then we have it
IF(LIBTS_INCLUDE_DIR AND LIBTS_LIBRARY)
  SET(LIBTS_FOUND "YES")
ENDIF(LIBTS_INCLUDE_DIR AND LIBTS_LIBRARY)

if( NOT WIN32)
  list(APPEND LIBTS_LIBRARY "-lrt")
endif( NOT WIN32)

MARK_AS_ADVANCED(
  LIBTS_FOUND
  LIBTS_LIBRARY
  LIBTS_LIBRARY_DIR
  LIBTS_INCLUDE_DIR
  LIBTS_PLUGIN_DIR
)
