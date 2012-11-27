# -*- mode: cmake; -*-
project(chumby)
cmake_minimum_required(VERSION 2.6.2 FATAL_ERROR)
if(POLICY CMP0011)
  cmake_policy(SET CMP0011 NEW)
endif(POLICY CMP0011)

set(CHUMBY_MAJOR_VERSION 0)
set(CHUMBY_MINOR_VERSION 1)
set(CHUMBY_SUB_VERSION   0)
set(VERSION_SEP "-")
set(CHUMBY_SHORT_VERSION "${CHUMBY_MAJOR_VERSION}${VERSION_SEP}${CHUMBY_MINOR_VERSION}")
set(CHUMBY_VERSION "${CHUMBY_SHORT_VERSION}${VERSION_SEP}${CHUMBY_SUB_VERSION}")
set(CHUMBY_RPM_VERSION "${CHUMBY_MAJOR_VERSION}.${CHUMBY_MINOR_VERSION}.${CHUMBY_SUB_VERSION}")
set(VERSION_DATE "2012-08-24")

set(PANEL_DIR "chumby-controlpanel/controlpanel")
set(PLUGIN_DIR "chumby-controlpanel/plugins")

###########################################################
# Where are the additional libraries installed? Note: provide includes
# path here, subsequent checks will resolve everything else
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/modules")

include ( CompilerFlags )
include(FindQt4)
include(UseQt4)
message(STATUS "QT: '${QT_LIBRARIES}'")
IF(NOT QT_FOUND)
  message(FATAL_ERROR "No QT4 found, cannot continue.")
ENDIF(NOT QT_FOUND)
include(Tools)

# search for alsa sound
find_package(ASound)

# search for iwlib
# search for libts
find_package(LibTS)

find_package(Atomix)
find_package(BlueTune)
find_package(Neptune)
IF(ATOMIX_FOUND AND BLUETUNE_FOUND AND NEPTUNE_FOUND)
  set(WITH_BLUETUNE "YES")
ELSE(ATOMIX_FOUND AND BLUETUNE_FOUND AND NEPTUNE_FOUND)
  message(WARNING "Warning: No working BlueTune found. Disabling features depending on BlueTune.")
ENDIF(ATOMIX_FOUND AND BLUETUNE_FOUND AND NEPTUNE_FOUND)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXXFLAGS}")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC --rtti -W -Wall -Wextra ")
#set(CMAKE_CC_FLAGS "${CMAKE_CC_FLAGS} -fPIC")

include_directories(${CMAKE_BINARY_DIR})
include_directories(${CMAKE_SOURCE_DIR})
include_directories(${${PROJECT_NAME}_SOURCE_DIR} ${${PROJECT_NAME}_BINARY_DIR})

include_directories(${TOOLCHAIN_PREFIX}/usr/include)
include_directories(${QT_INCLUDES})
link_directories(${QT_LIBRARY_DIR})
link_directories(${TOOLCHAIN_PREFIX}/usr/lib)

add_subdirectory(controlpanel)
add_subdirectory(plugins)
add_subdirectory(tools)

# enable unit testing
include(CTest)
enable_testing()

### print some output for the user
message("")
message("        ***** Configuration parameters *****")
message("             prefix: ${CMAKE_INSTALL_PREFIX}")

# add some files to the installation target
INSTALL(FILES README.md DESTINATION 
  share/doc/chumby-${CHUMBY_MAJOR_VERSION}-${CHUMBY_MINOR_VERSION} RENAME README
)
INSTALL(FILES INSTALL COPYRIGHT DESTINATION
  share/doc/chumby-${CHUMBY_MAJOR_VERSION}-${CHUMBY_MINOR_VERSION}
)

# CPACK packaging
INCLUDE(InstallRequiredSystemLibraries)
set(CPACK_SOURCE_GENERATOR "TGZ;TBZ2")

set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Gridlicense Demo Tools")
set(CPACK_PACKAGE_VENDOR              "Fraunhofer ITWM")
set(CPACK_PACKAGE_CONTACT             "Kai Krueger <krueger@itwm.fhg.de>")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER   "Kai Krueger <krueger@itwm.fhg.de>")
set(CPACK_PACKAGE_DESCRIPTION_FILE    "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/COPYRIGHT")
set(CPACK_PACKAGE_VERSION_MAJOR ${CHUMBY_MAJOR_VERSION})
set(CPACK_PACKAGE_VERSION_MINOR ${CHUMBY_MINOR_VERSION})
set(CPACK_PACKAGE_VERSION_PATCH ${CHUMBY_SUB_VERSION})
set(CPACK_PACKAGE_INSTALL_DIRECTORY "${PROJECT_NAME}-${CHUMBY_MAJOR_VERSION}.${CHUMBY_MINOR_VERSION}")

set(CPACK_SOURCE_IGNORE_FILES "/CVS/;/.svn/;.swp$;.#;/#;.*~;cscope.*;build.*")

IF(WIN32 AND NOT UNIX)
  set(CPACK_GENERATOR "NSIS")
  set(CPACK_NSIS_PACKAGE_NAME "Gridlicense Demo Tools")
  # set(CPACK_PACKAGE_EXECUTABLES "gui4" "Grid License Demo Frontend")
  set(CPACK_PACKAGE_DEFAULT_LOCATION ${INSTALL_DIR})

  # sure there is at least one set of four (4) backlasshes.
  set(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\gui4.exe")
  set(CPACK_NSIS_DISPLAY_NAME "${CPACK_PACKAGE_INSTALL_DIRECTORY} Gridlicense Demo Tools")
  set(CPACK_NSIS_HELP_LINK    "http://www.itwm.fraunhofer.de/")
  set(CPACK_NSIS_URL_INFO_ABOUT "http://www.itwm.fraunhofer.de/abteilungen/hpc.html")
  set(CPACK_NSIS_CONTACT "Kai Krueger <krueger@itwm.fhg.de>")
  set(CPACK_NSIS_MODIFY_PATH ON)
  # set(CPACK_PACKAGE_INSTALL_REGISTRY_KEY "${CMAKE_PROJECT_NAME}-${EWS_VERSION}")
  # set(CPACK_NSIS_MENU_LINKS "${EWS_WEBSITE}" "Homepage for ${EWS_APP_NAME}")
  set(CPACK_NSIS_DEFINES "  !define MUI_WELCOMEPAGE_TITLE_3LINES 1
    !define MUI_FINISHPAGE_TITLE_3LINES 1")
else(WIN32 AND NOT UNIX)
  #set(CPACK_SET_DESTDIR On)
  set(CPACK_SOURCE_STRIP_FILES "")
  set(CPACK_GENERATOR "TGZ;TBZ2;ZIP")
  set(CPACK_RPM_PACKAGE_RELOCATABLE 1)
endif(WIN32 AND NOT UNIX)
include(CPack)

