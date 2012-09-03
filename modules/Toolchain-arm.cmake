# -*- mode: cmake; -*-

if(NOT TOOLCHAIN_PREFIX)
     set(TOOLCHAIN_PREFIX "/opt/arm-2008q3")
     message(STATUS "No TOOLCHAIN_PREFIX specified, using default: " ${TOOLCHAIN_PREFIX})
endif()

if(NOT TARGET_TRIPLET)
    set(TARGET_TRIPLET "arm-none-linux-gnueabi")
    message(STATUS "No TARGET_TRIPLET specified, using default: " ${TARGET_TRIPLET})
endif()

set(TOOLCHAIN_BIN_DIR ${TOOLCHAIN_PREFIX}/bin)
set(TOOLCHAIN_INC_DIR ${TOOLCHAIN_PREFIX}/${TARGET_TRIPLET}/include)
set(TOOLCHAIN_LIB_DIR ${TOOLCHAIN_PREFIX}/${TARGET_TRIPLET}/lib)

set(CMAKE_SYSTEM_NAME Linux CACHE INTERNAL "system name")
set(CMAKE_SYSTEM_PROCESSOR arm CACHE INTERNAL "processor")

set(_c_compiler   ${TARGET_TRIPLET}-gcc)
set(_cxx_compiler ${TARGET_TRIPLET}-g++)
set(_asm_compiler ${TARGET_TRIPLET}-as)
#set(QTDIR "${TOOLCHAIN_PREFIX}/usr/qt4")
set(QTDIR "/mnt/usb")
set(ENV{QTDIR} ${QTDIR})

#compiler
find_program(c_compiler
  NAMES ${_c_compiler}
  HINTS ${TOOLCHAIN_PREFIX}
  PATH_SUFFIXES bin
  DOC "find gcc compiler"
  NO_DEFAULT_PATH
  NO_CMAKE_ENVIRONMENT_PATH
  NO_CMAKE_PATH
  NO_SYSTEM_ENVIRONMENT_PATH
  NO_CMAKE_SYSTEM_PATH
)
if( ${c_compiler} STREQUAL "c_compiler-NOTFOUND" )
  message(FATAL_ERROR  "OpenWRT cross c-compiler ${c_compiler} not found.")
endif( ${c_compiler} STREQUAL "c_compiler-NOTFOUND" )

find_program(cxx_compiler
  NAMES ${_cxx_compiler}
  HINTS ${TOOLCHAIN_PREFIX}
  PATH_SUFFIXES bin
  DOC "find g++ compiler"
  NO_DEFAULT_PATH
  NO_CMAKE_ENVIRONMENT_PATH
  NO_CMAKE_PATH
  NO_SYSTEM_ENVIRONMENT_PATH
  NO_CMAKE_SYSTEM_PATH
)
if( ${cxx_compiler} STREQUAL "cxx_compiler-NOTFOUND" )
  message(FATAL_ERROR  "Arm cross c++-compiler ${cxx_compiler} not found.")
endif( ${cxx_compiler} STREQUAL "cxx_compiler-NOTFOUND" )

find_program(asm_compiler
  NAMES ${_asm_compiler}
  HINTS ${TOOLCHAIN_PREFIX}
  PATH_SUFFIXES bin
  DOC "find g++ compiler"
  NO_DEFAULT_PATH
  NO_CMAKE_ENVIRONMENT_PATH
  NO_CMAKE_PATH
  NO_SYSTEM_ENVIRONMENT_PATH
  NO_CMAKE_SYSTEM_PATH
)
if( ${asm_compiler} STREQUAL "asm_compiler-NOTFOUND" )
  message(FATAL_ERROR  "Arm cross c++-compiler ${asm_compiler} not found.")
endif( ${asm_compiler} STREQUAL "asm_compiler-NOTFOUND" )

set(CMAKE_C_COMPILER   ${c_compiler}   CACHE INTERNAL "c compiler")
set(CMAKE_CXX_COMPILER ${cxx_compiler} CACHE INTERNAL "cxx compiler")
set(CMAKE_ASM_COMPILER ${asm_compiler} CACHE INTERNAL "asm compiler")

set(CMAKE_OBJCOPY ${TOOLCHAIN_BIN_DIR}/${TARGET_TRIPLET}-objcopy CACHE INTERNAL "objcopy")
set(CMAKE_OBJDUMP ${TOOLCHAIN_BIN_DIR}/${TARGET_TRIPLET}-objdump CACHE INTERNAL "objdump")

set(ENV{PATH}            ${TOOLCHAIN_PREFIX}/bin:$ENV{PATH})
set(ENV{PATH}            ${QTDIR}/bin:$ENV{PATH})
message(STATUS "export PATH=$ENV{PATH}")

set(CPACK_ARCHITECTUR "arm")
set(CPACK_SYSTEM_NAME "arm")
set(CPACK_BINARY_DIR "${CMAKE_BINARY_DIR}/_CPack_Packages/")


# where is the target environment 
SET(CMAKE_FIND_ROOT_PATH ${ARM_HOME}  ${ARM_TARGET_DIR} ${ARM_TOOLCHAIN_DIR} /tmp)

# search for programs in the build host directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY NEVER) #ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE NEVER) # ONLY)

#set(CMAKE_MODULE_PATH ${ARM_STAGING_DIR_HOST}/Modules ${CMAKE_MODULE_PATH})
#set(CPACK_MODULE_PATH ${ARM_STAGING_DIR_HOST}/Modules ${CPACK_MODULE_PATH})

message(STATUS "Arm staging....: '${ARM_STAGING_DIR}'")
message(STATUS "Arm toolchain..: '${ARM_TOOLCHAIN_DIR}'")
message(STATUS "Arm target.....: '${ARM_TARGET_DIR}'")
message(STATUS "Arm gcc .......: '${CMAKE_C_COMPILER}'")
message(STATUS "Arm g++ .......: '${CMAKE_CXX_COMPILER}'")
message(STATUS "Arm QT4 .......: '${QTDIR}'")
message(STATUS "Arm QT4 .......: '$ENV{QTDIR}'")
