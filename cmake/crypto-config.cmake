# CRYPRO CMake configuration file:
# This file is meant to be placed in a cmake subfolder of CRYPRO-devel-2.x.y-VC

cmake_minimum_required(VERSION 3.0)

include(FeatureSummary)
set_package_properties(CRYPRO PROPERTIES
    URL "https://www.g10.app/status"
    DESCRIPTION "crypto"
)

# Copied from `configure_package_config_file`
macro(set_and_check _var _file)
    set(${_var} "${_file}")
    if(NOT EXISTS "${_file}")
        message(FATAL_ERROR "File or directory ${_file} referenced by variable ${_var} does not exist !")
    endif()
endmacro()

# Copied from `configure_package_config_file`
macro(check_required_components _NAME)
    foreach(comp ${${_NAME}_FIND_COMPONENTS})
        if(NOT ${_NAME}_${comp}_FOUND)
            if(${_NAME}_FIND_REQUIRED_${comp})
                set(${_NAME}_FOUND FALSE)
            endif()
        endif()
    endforeach()
endmacro()

set(CRYPRO_FOUND TRUE)

# For compatibility with autotools CRYPRO-config.cmake, provide CRYPRO_* variables.

set_and_check(CRYPRO_PREFIX       "${CMAKE_CURRENT_LIST_DIR}/..")
set_and_check(CRYPRO_EXEC_PREFIX  "${CMAKE_CURRENT_LIST_DIR}/..")
set_and_check(CRYPRO_INCLUDE_DIR  "${CRYPRO_PREFIX}/include")
set(CRYPRO_INCLUDE_DIRS           "${CRYPRO_INCLUDE_DIR}")
set_and_check(CRYPRO_BINDIR       "${CMAKE_CURRENT_LIST_DIR}/../build/Debug/")
set_and_check(CRYPRO_LIBDIR       "${CMAKE_CURRENT_LIST_DIR}/../build/Debug/")

set(CRYPRO_LIBRARIES crypto::crypto)

# All targets are created, even when some might not be requested though COMPONENTS.
# This is done for compatibility with CMake generated CRYPRO-target.cmake files.

set(_CRYPRO_library     "${CRYPRO_LIBDIR}/crypto.lib")
set(_CRYPRO_dll_library "${CRYPRO_BINDIR}/crypto.dll")
if(EXISTS "${_CRYPRO_library}" AND EXISTS "${_CRYPRO_dll_library}")
    if(NOT TARGET crypto::crypto)
        add_library(crypto::crypto SHARED IMPORTED)
        set_target_properties(crypto::crypto
            PROPERTIES
                INTERFACE_INCLUDE_DIRECTORIES "${CRYPRO_INCLUDE_DIRS}"
                IMPORTED_IMPLIB "${_CRYPRO_library}"
                IMPORTED_LOCATION "${_CRYPRO_dll_library}"
                COMPATIBLE_INTERFACE_BOOL "CRYPRO_SHARED"
                INTERFACE_CRYPRO_SHARED "ON"
        )
    endif()
    set(CRYPRO_CRYPRO_FOUND TRUE)
else()
    set(CRYPRO_CRYPRO_FOUND FALSE)
endif()
unset(_CRYPRO_library)
unset(_CRYPRO_dll_library)

check_required_components(CRYPRO)
