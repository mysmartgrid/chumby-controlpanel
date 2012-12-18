# -*- mode: cmake; -*-
# - Try to find BlueTune include dirs and libraries
# Usage of this module as follows:
# This file defines:
# * BLUETUNE_FOUND if BlueTune was found
# * BLUETUNE_LIBRARY The lib to link to (currently only a static unix lib, not
# portable)
# * BLUETUNE_INCLUDE The include directories for BlueTune.

message(STATUS "FindBlueTune check")
#IF (NOT WIN32)
#  include(FindPkgConfig)
#  if ( PKG_CONFIG_FOUND )
#
#     pkg_check_modules (PC_BLUETUNE alsa>=1.0)
#
#     set(BLUETUNE_DEFINITIONS ${PC_BLUETUNE_CFLAGS_OTHER})
#  endif(PKG_CONFIG_FOUND)
#endif (NOT WIN32)

#
# set defaults
SET(_bluetune_HOME "/usr/local")
SET(_bluetune_INCLUDE_SEARCH_DIRS
  ${CMAKE_INCLUDE_PATH}
  /usr/local/include
  /usr/include
  )

SET(_bluetune_LIBRARIES_SEARCH_DIRS
  ${CMAKE_LIBRARY_PATH}
  /usr/local/lib
  /usr/lib
  )

##
if( "${BLUETUNE_HOME}" STREQUAL "")
  if("" MATCHES "$ENV{BLUETUNE_HOME}")
    message(STATUS "BLUETUNE_HOME env is not set, setting it to /usr/local")
    set (BLUETUNE_HOME ${_bluetune_HOME})
  else("" MATCHES "$ENV{BLUETUNE_HOME}")
    set (BLUETUNE_HOME "$ENV{BLUETUNE_HOME}")
  endif("" MATCHES "$ENV{BLUETUNE_HOME}")
else( "${BLUETUNE_HOME}" STREQUAL "")
  message(STATUS "BLUETUNE_HOME is not empty: \"${BLUETUNE_HOME}\"")
endif( "${BLUETUNE_HOME}" STREQUAL "")
##

message(STATUS "Looking for bluetune in ${BLUETUNE_HOME}")

IF( NOT ${BLUETUNE_HOME} STREQUAL "" )
    SET(_bluetune_INCLUDE_SEARCH_DIRS ${BLUETUNE_HOME}/include ${_bluetune_INCLUDE_SEARCH_DIRS})
    SET(_bluetune_LIBRARIES_SEARCH_DIRS ${BLUETUNE_HOME}/lib ${_bluetune_LIBRARIES_SEARCH_DIRS})
    SET(_bluetune_HOME ${BLUETUNE_HOME})
ENDIF( NOT ${BLUETUNE_HOME} STREQUAL "" )

IF( NOT $ENV{BLUETUNE_INCLUDEDIR} STREQUAL "" )
  SET(_bluetune_INCLUDE_SEARCH_DIRS $ENV{BLUETUNE_INCLUDEDIR} ${_bluetune_INCLUDE_SEARCH_DIRS})
ENDIF( NOT $ENV{BLUETUNE_INCLUDEDIR} STREQUAL "" )

IF( NOT $ENV{BLUETUNE_LIBRARYDIR} STREQUAL "" )
  SET(_bluetune_LIBRARIES_SEARCH_DIRS $ENV{BLUETUNE_LIBRARYDIR} ${_bluetune_LIBRARIES_SEARCH_DIRS})
ENDIF( NOT $ENV{BLUETUNE_LIBRARYDIR} STREQUAL "" )

IF( BLUETUNE_HOME )
  SET(_bluetune_INCLUDE_SEARCH_DIRS ${BLUETUNE_HOME}/include/BlueTune ${_bluetune_INCLUDE_SEARCH_DIRS})
  SET(_bluetune_LIBRARIES_SEARCH_DIRS ${BLUETUNE_HOME}/lib ${_bluetune_LIBRARIES_SEARCH_DIRS})
  SET(_bluetune_HOME ${BLUETUNE_HOME})
ENDIF( BLUETUNE_HOME )

# find the include files
FIND_PATH(BLUETUNE_INCLUDE_DIR BlueTune/BlueTune.h
   HINTS
     ${_bluetune_INCLUDE_SEARCH_DIRS}
     ${PC_BLUETUNE_INCLUDEDIR}
     ${PC_BLUETUNE_INCLUDE_DIRS}
    ${CMAKE_INCLUDE_PATH}
)

SET(BLUETUNE_INCLUDE_DIRS
  ${BLUETUNE_INCLUDE_DIR}/BlueTune
  ${BLUETUNE_INCLUDE_DIR}/Core
  ${BLUETUNE_INCLUDE_DIR}/Decoder
  ${BLUETUNE_INCLUDE_DIR}/Player
)

# locate the library
IF(WIN32)
  SET(BLUETUNE_LIBRARY_NAMES ${BLUETUNE_LIBRARY_NAMES} libBlueTune.lib)
ELSE(WIN32)
  SET(BLUETUNE_LIBRARY_NAMES ${BLUETUNE_LIBRARY_NAMES} libBlueTune.a)
ENDIF(WIN32)
FIND_LIBRARY(BLUETUNE_LIBRARY NAMES ${BLUETUNE_LIBRARY_NAMES}
  HINTS
    ${_bluetune_LIBRARIES_SEARCH_DIRS}
    ${PC_BLUETUNE_LIBDIR}
    ${PC_BLUETUNE_LIBRARY_DIRS}
)

# if the include and the program are found then we have it
IF(BLUETUNE_INCLUDE_DIR AND BLUETUNE_LIBRARY)
  SET(BLUETUNE_FOUND "YES")
ENDIF(BLUETUNE_INCLUDE_DIR AND BLUETUNE_LIBRARY)

if( NOT WIN32)
  list(APPEND BLUETUNE_LIBRARY "-lrt")
endif( NOT WIN32)

MARK_AS_ADVANCED(
  BLUETUNE_FOUND
  BLUETUNE_LIBRARY
  BLUETUNE_INCLUDE_DIR
  BLUETUNE_INCLUDE_DIRS
)
