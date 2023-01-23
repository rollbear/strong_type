#
# strong_type C++14/17/20 strong typedef library
#
# Copyright (C) Björn Fahller
#
#  Use, modification and distribution is subject to the
#  Boost Software License, Version 1.0. (See accompanying
#  file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)
#
# Project home: https://github.com/rollbear/strong_type
#


cmake_minimum_required(VERSION 3.7)
project(strong_type)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

option(STRONG_TYPE_UNIT_TEST off "Decide whether to build unit tests or not")

set(STRONG_TYPE_VERSION 10)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(MASTER_PROJECT OFF)
if (${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_SOURCE_DIR})
  set(MASTER_PROJECT ON)
endif()

if (${STRONG_TYPE_UNIT_TEST})
  add_subdirectory(test)
endif()

write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/strong_type/strong_type-config-version.cmake"
  VERSION ${STRONG_TYPE_VERSION}
  COMPATIBILITY AnyNewerVersion)

add_library(strong_type INTERFACE)
add_library(strong_type::strong_type ALIAS strong_type)

target_include_directories(
  strong_type
  INTERFACE
  $<BUILD_INTERFACE:${INCLUDE_DIR}>
)

target_include_directories(
  strong_type
  INTERFACE
  $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>
)

install(
  TARGETS
    strong_type
  EXPORT
    strong_type-targets
  INCLUDES DESTINATION
    include
)

install(
  EXPORT
    strong_type-targets
  NAMESPACE
    strong_type::
  DESTINATION
    lib/cmake/strong_type
)
install(
  FILES
    strong_type-config.cmake
    "${CMAKE_CURRENT_BINARY_DIR}/strong_type/strong_type-config-version.cmake"
  DESTINATION
    lib/cmake/strong_type
  COMPONENT
    Devel
)

install(
  DIRECTORY
    "include/strong_type/"
  DESTINATION
    "${CMAKE_INSTALL_INCLUDEDIR}/strong_type"
)

