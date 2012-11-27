# -*- mode: cmake; -*-
# - Try to find Neptune include dirs and libraries
# Usage of this module as follows:
# This file defines:
# * NEPTUNE_FOUND if Neptune was found
# * NEPTUNE_LIBRARY The lib to link to (currently only a static unix lib, not
# portable)
# * NEPTUNE_INCLUDE The include directories for Neptune.

message(STATUS "FindNeptune check")
#IF (NOT WIN32)
#  include(FindPkgConfig)
#  if ( PKG_CONFIG_FOUND )
#
#     pkg_check_modules (PC_NEPTUNE alsa>=1.0)
#
#     set(NEPTUNE_DEFINITIONS ${PC_NEPTUNE_CFLAGS_OTHER})
#  endif(PKG_CONFIG_FOUND)
#endif (NOT WIN32)

#
# set defaults
SET(_neptune_HOME "/usr/local")
SET(_neptune_INCLUDE_SEARCH_DIRS
  ${CMAKE_INCLUDE_PATH}
  /usr/local/include
  /usr/include
  )

SET(_neptune_LIBRARIES_SEARCH_DIRS
  ${CMAKE_LIBRARY_PATH}
  /usr/local/lib
  /usr/lib
  )

##
if( "${NEPTUNE_HOME}" STREQUAL "")
  if("" MATCHES "$ENV{NEPTUNE_HOME}")
    message(STATUS "NEPTUNE_HOME env is not set, setting it to /usr/local")
    set (NEPTUNE_HOME ${_neptune_HOME})
  else("" MATCHES "$ENV{NEPTUNE_HOME}")
    set (NEPTUNE_HOME "$ENV{NEPTUNE_HOME}")
  endif("" MATCHES "$ENV{NEPTUNE_HOME}")
else( "${NEPTUNE_HOME}" STREQUAL "")
  message(STATUS "NEPTUNE_HOME is not empty: \"${NEPTUNE_HOME}\"")
endif( "${NEPTUNE_HOME}" STREQUAL "")
##

message(STATUS "Looking for neptune in ${NEPTUNE_HOME}")

IF( NOT ${NEPTUNE_HOME} STREQUAL "" )
    SET(_neptune_INCLUDE_SEARCH_DIRS ${NEPTUNE_HOME}/include ${_neptune_INCLUDE_SEARCH_DIRS})
    SET(_neptune_LIBRARIES_SEARCH_DIRS ${NEPTUNE_HOME}/lib ${_neptune_LIBRARIES_SEARCH_DIRS})
    SET(_neptune_HOME ${NEPTUNE_HOME})
ENDIF( NOT ${NEPTUNE_HOME} STREQUAL "" )

IF( NOT $ENV{NEPTUNE_INCLUDEDIR} STREQUAL "" )
  SET(_neptune_INCLUDE_SEARCH_DIRS $ENV{NEPTUNE_INCLUDEDIR} ${_neptune_INCLUDE_SEARCH_DIRS})
ENDIF( NOT $ENV{NEPTUNE_INCLUDEDIR} STREQUAL "" )

IF( NOT $ENV{NEPTUNE_LIBRARYDIR} STREQUAL "" )
  SET(_neptune_LIBRARIES_SEARCH_DIRS $ENV{NEPTUNE_LIBRARYDIR} ${_neptune_LIBRARIES_SEARCH_DIRS})
ENDIF( NOT $ENV{NEPTUNE_LIBRARYDIR} STREQUAL "" )

IF( NEPTUNE_HOME )
  SET(_neptune_INCLUDE_SEARCH_DIRS ${NEPTUNE_HOME}/include/Neptune/Core ${_neptune_INCLUDE_SEARCH_DIRS})
  SET(_neptune_LIBRARIES_SEARCH_DIRS ${NEPTUNE_HOME}/lib ${_neptune_LIBRARIES_SEARCH_DIRS})
  SET(_neptune_HOME ${NEPTUNE_HOME})
ENDIF( NEPTUNE_HOME )

# find the include files
FIND_PATH(NEPTUNE_INCLUDE_DIR Neptune.h
   HINTS
     ${_neptune_INCLUDE_SEARCH_DIRS}
     ${PC_NEPTUNE_INCLUDEDIR}
     ${PC_NEPTUNE_INCLUDE_DIRS}
    ${CMAKE_INCLUDE_PATH}
)

# locate the library
IF(WIN32)
  SET(NEPTUNE_LIBRARY_NAMES ${NEPTUNE_LIBRARY_NAMES} libNeptune.lib)
ELSE(WIN32)
  SET(NEPTUNE_LIBRARY_NAMES ${NEPTUNE_LIBRARY_NAMES} libNeptune.a)
ENDIF(WIN32)
FIND_LIBRARY(NEPTUNE_LIBRARY NAMES ${NEPTUNE_LIBRARY_NAMES}
  HINTS
    ${_neptune_LIBRARIES_SEARCH_DIRS}
    ${PC_NEPTUNE_LIBDIR}
    ${PC_NEPTUNE_LIBRARY_DIRS}
)

# if the include and the program are found then we have it
IF(NEPTUNE_INCLUDE_DIR AND NEPTUNE_LIBRARY)
  SET(NEPTUNE_FOUND "YES")
  message(STATUS "FindNeptune check - found")
ELSE(NEPTUNE_INCLUDE_DIR AND NEPTUNE_LIBRARY)
  message(STATUS "FindNeptune check - not found")
ENDIF(NEPTUNE_INCLUDE_DIR AND NEPTUNE_LIBRARY)

if( NOT WIN32)
  list(APPEND NEPTUNE_LIBRARY "-lrt")
endif( NOT WIN32)

MARK_AS_ADVANCED(
  NEPTUNE_FOUND
  NEPTUNE_LIBRARY
  NEPTUNE_INCLUDE_DIR
)
