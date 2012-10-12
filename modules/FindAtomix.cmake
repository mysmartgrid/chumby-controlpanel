# -*- mode: cmake; -*-
# - Try to find Atomix include dirs and libraries
# Usage of this module as follows:
# This file defines:
# * ATOMIX_FOUND if Atomix was found
# * ATOMIX_LIBRARY The lib to link to (currently only a static unix lib, not
# portable)
# * ATOMIX_INCLUDE The include directories for Atomix.

message(STATUS "FindAtomix check")
#IF (NOT WIN32)
#  include(FindPkgConfig)
#  if ( PKG_CONFIG_FOUND )
#
#     pkg_check_modules (PC_ATOMIX alsa>=1.0)
#
#     set(ATOMIX_DEFINITIONS ${PC_ATOMIX_CFLAGS_OTHER})
#  endif(PKG_CONFIG_FOUND)
#endif (NOT WIN32)

#
# set defaults
SET(_atomix_HOME "/usr/local")
SET(_atomix_INCLUDE_SEARCH_DIRS
  ${CMAKE_INCLUDE_PATH}
  /usr/local/include
  /usr/include
  )

SET(_atomix_LIBRARIES_SEARCH_DIRS
  ${CMAKE_LIBRARY_PATH}
  /usr/local/lib
  /usr/lib
  )

##
if( "${ATOMIX_HOME}" STREQUAL "")
  if("" MATCHES "$ENV{ATOMIX_HOME}")
    message(STATUS "ATOMIX_HOME env is not set, setting it to /usr/local")
    set (ATOMIX_HOME ${_atomix_HOME})
  else("" MATCHES "$ENV{ATOMIX_HOME}")
    set (ATOMIX_HOME "$ENV{ATOMIX_HOME}")
  endif("" MATCHES "$ENV{ATOMIX_HOME}")
else( "${ATOMIX_HOME}" STREQUAL "")
  message(STATUS "ATOMIX_HOME is not empty: \"${ATOMIX_HOME}\"")
endif( "${ATOMIX_HOME}" STREQUAL "")
##

message(STATUS "Looking for atomix in ${ATOMIX_HOME}")

IF( NOT ${ATOMIX_HOME} STREQUAL "" )
    SET(_atomix_INCLUDE_SEARCH_DIRS ${ATOMIX_HOME}/include ${_atomix_INCLUDE_SEARCH_DIRS})
    SET(_atomix_LIBRARIES_SEARCH_DIRS ${ATOMIX_HOME}/lib ${_atomix_LIBRARIES_SEARCH_DIRS})
    SET(_atomix_HOME ${ATOMIX_HOME})
ENDIF( NOT ${ATOMIX_HOME} STREQUAL "" )

IF( NOT $ENV{ATOMIX_INCLUDEDIR} STREQUAL "" )
  SET(_atomix_INCLUDE_SEARCH_DIRS $ENV{ATOMIX_INCLUDEDIR} ${_atomix_INCLUDE_SEARCH_DIRS})
ENDIF( NOT $ENV{ATOMIX_INCLUDEDIR} STREQUAL "" )

IF( NOT $ENV{ATOMIX_LIBRARYDIR} STREQUAL "" )
  SET(_atomix_LIBRARIES_SEARCH_DIRS $ENV{ATOMIX_LIBRARYDIR} ${_atomix_LIBRARIES_SEARCH_DIRS})
ENDIF( NOT $ENV{ATOMIX_LIBRARYDIR} STREQUAL "" )

IF( ATOMIX_HOME )
  SET(_atomix_INCLUDE_SEARCH_DIRS ${ATOMIX_HOME}/include/Atomix/Core ${_atomix_INCLUDE_SEARCH_DIRS})
  SET(_atomix_LIBRARIES_SEARCH_DIRS ${ATOMIX_HOME}/lib ${_atomix_LIBRARIES_SEARCH_DIRS})
  SET(_atomix_HOME ${ATOMIX_HOME})
ENDIF( ATOMIX_HOME )

# find the include files
FIND_PATH(ATOMIX_INCLUDE_DIR Atomix.h
   HINTS
     ${_atomix_INCLUDE_SEARCH_DIRS}
     ${PC_ATOMIX_INCLUDEDIR}
     ${PC_ATOMIX_INCLUDE_DIRS}
    ${CMAKE_INCLUDE_PATH}
)

# locate the library
IF(WIN32)
  SET(ATOMIX_LIBRARY_NAMES ${ATOMIX_LIBRARY_NAMES} libAtomix.lib)
ELSE(WIN32)
  SET(ATOMIX_LIBRARY_NAMES ${ATOMIX_LIBRARY_NAMES} libAtomix.a)
ENDIF(WIN32)
FIND_LIBRARY(ATOMIX_LIBRARY NAMES ${ATOMIX_LIBRARY_NAMES}
  HINTS
    ${_atomix_LIBRARIES_SEARCH_DIRS}
    ${PC_ATOMIX_LIBDIR}
    ${PC_ATOMIX_LIBRARY_DIRS}
)

# if the include and the program are found then we have it
IF(ATOMIX_INCLUDE_DIR AND ATOMIX_LIBRARY)
  SET(ATOMIX_FOUND "YES")
ENDIF(ATOMIX_INCLUDE_DIR AND ATOMIX_LIBRARY)

if( NOT WIN32)
  list(APPEND ATOMIX_LIBRARY "-lrt")
endif( NOT WIN32)

MARK_AS_ADVANCED(
  ATOMIX_FOUND
  ATOMIX_LIBRARY
  ATOMIX_INCLUDE_DIR
)
