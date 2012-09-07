# -*- mode: cmake; -*-
# - Try to find libasound include dirs and libraries
# Usage of this module as follows:
# This file defines:
# * ASOUND_FOUND if protoc was found
# * ASOUND_LIBRARY The lib to link to (currently only a static unix lib, not
# portable)
# * ASOUND_INCLUDE The include directories for libasound.

message(STATUS "FindAsound check")
IF (NOT WIN32)
  include(FindPkgConfig)
  if ( PKG_CONFIG_FOUND )

     pkg_check_modules (PC_ASOUND alsa>=1.0)

     set(ASOUND_DEFINITIONS ${PC_ASOUND_CFLAGS_OTHER})
  endif(PKG_CONFIG_FOUND)
endif (NOT WIN32)

#
# set defaults
SET(_asound_HOME "/usr/local")
SET(_asound_INCLUDE_SEARCH_DIRS
  ${CMAKE_INCLUDE_PATH}
  /usr/local/include
  /usr/include
  )

SET(_asound_LIBRARIES_SEARCH_DIRS
  ${CMAKE_LIBRARY_PATH}
  /usr/local/lib
  /usr/lib
  )

##
if( "${ASOUND_HOME}" STREQUAL "")
  if("" MATCHES "$ENV{ASOUND_HOME}")
    message(STATUS "ASOUND_HOME env is not set, setting it to /usr/local")
    set (ASOUND_HOME ${_asound_HOME})
  else("" MATCHES "$ENV{ASOUND_HOME}")
    set (ASOUND_HOME "$ENV{ASOUND_HOME}")
  endif("" MATCHES "$ENV{ASOUND_HOME}")
else( "${ASOUND_HOME}" STREQUAL "")
  message(STATUS "ASOUND_HOME is not empty: \"${ASOUND_HOME}\"")
endif( "${ASOUND_HOME}" STREQUAL "")
##

message(STATUS "Looking for asound in ${ASOUND_HOME}")

IF( NOT ${ASOUND_HOME} STREQUAL "" )
    SET(_asound_INCLUDE_SEARCH_DIRS ${ASOUND_HOME}/include ${_asound_INCLUDE_SEARCH_DIRS})
    SET(_asound_LIBRARIES_SEARCH_DIRS ${ASOUND_HOME}/lib ${_asound_LIBRARIES_SEARCH_DIRS})
    SET(_asound_HOME ${ASOUND_HOME})
ENDIF( NOT ${ASOUND_HOME} STREQUAL "" )

IF( NOT $ENV{ASOUND_INCLUDEDIR} STREQUAL "" )
  SET(_asound_INCLUDE_SEARCH_DIRS $ENV{ASOUND_INCLUDEDIR} ${_asound_INCLUDE_SEARCH_DIRS})
ENDIF( NOT $ENV{ASOUND_INCLUDEDIR} STREQUAL "" )

IF( NOT $ENV{ASOUND_LIBRARYDIR} STREQUAL "" )
  SET(_asound_LIBRARIES_SEARCH_DIRS $ENV{ASOUND_LIBRARYDIR} ${_asound_LIBRARIES_SEARCH_DIRS})
ENDIF( NOT $ENV{ASOUND_LIBRARYDIR} STREQUAL "" )

IF( ASOUND_HOME )
  SET(_asound_INCLUDE_SEARCH_DIRS ${ASOUND_HOME}/include ${_asound_INCLUDE_SEARCH_DIRS})
  SET(_asound_LIBRARIES_SEARCH_DIRS ${ASOUND_HOME}/lib ${_asound_LIBRARIES_SEARCH_DIRS})
  SET(_asound_HOME ${ASOUND_HOME})
ENDIF( ASOUND_HOME )

# find the include files
FIND_PATH(ASOUND_INCLUDE_DIR alsa/asoundlib.h
   HINTS
     ${_asound_INCLUDE_SEARCH_DIRS}
     ${PC_ASOUND_INCLUDEDIR}
     ${PC_ASOUND_INCLUDE_DIRS}
    ${CMAKE_INCLUDE_PATH}
)

# locate the library
IF(WIN32)
  SET(ASOUND_LIBRARY_NAMES ${ASOUND_LIBRARY_NAMES} libasound.lib)
ELSE(WIN32)
  SET(ASOUND_LIBRARY_NAMES ${ASOUND_LIBRARY_NAMES} libasound.a libasound.so)
ENDIF(WIN32)
FIND_LIBRARY(ASOUND_LIBRARY NAMES ${ASOUND_LIBRARY_NAMES}
  HINTS
    ${_asound_LIBRARIES_SEARCH_DIRS}
    ${PC_ASOUND_LIBDIR}
    ${PC_ASOUND_LIBRARY_DIRS}
)

# if the include and the program are found then we have it
IF(ASOUND_INCLUDE_DIR AND ASOUND_LIBRARY)
  SET(ASOUND_FOUND "YES")
ENDIF(ASOUND_INCLUDE_DIR AND ASOUND_LIBRARY)

if( NOT WIN32)
  list(APPEND ASOUND_LIBRARY "-lrt")
endif( NOT WIN32)

MARK_AS_ADVANCED(
  ASOUND_FOUND
  ASOUND_LIBRARY
  ASOUND_INCLUDE_DIR
)
