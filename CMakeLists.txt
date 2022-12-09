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


cmake_minimum_required(VERSION 3.19)
project(strong_type)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

set(STRONG_TYPE_VERSION 6)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(MASTER_PROJECT OFF)
if (${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_SOURCE_DIR})
  set(MASTER_PROJECT ON)
endif()

if (MASTER_PROJECT AND CMAKE_BUILD_TYPE MATCHES Debug)
  if (!CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 14)
  endif()
  set(CMAKE_CXX_STANDARD_REQUIRED YES)
  set(CMAKE_CXX_EXTENSIONS OFF)

  find_package(Catch2 3 QUIET)
  if (${Catch2_FOUND})
    add_compile_definitions(CATCH2=3)
  else ()
      find_package(Catch2 2 QUIET)
  endif()
  if (TARGET Catch2::Catch2WithMain)
    set(CATCHLIBS Catch2::Catch2WithMain)
  endif()
  if (NOT ${Catch2_FOUND})
    set(TEST_MAIN "test_main.cpp")
    set(CATCH_DIR ${CMAKE_CURRENT_BINARY_DIR}/catch)
    if(NOT EXISTS ${CATCH_DIR}/catch2/catch.hpp)
      if (NOT EXISTS ${CATCH_DIR}/catch2)
        file(MAKE_DIRECTORY "${CATCH_DIR}/catch2")
      endif()
      file(
        DOWNLOAD
          https://github.com/catchorg/Catch2/releases/download/v2.13.9/catch.hpp  ${CATCH_DIR}/catch2/catch.hpp
        STATUS
          status
        LOG
          log
      )
      list(GET status 0 status_code)
      list(GET status 1 status_string)

      if(NOT status_code EQUAL 0)
        message(FATAL_ERROR "error downloading catch: ${status_string}"
          "${log}")
      endif()

    endif()
    add_library(Catch2 INTERFACE)
    add_library(Catch2::Catch2 ALIAS Catch2)
    target_include_directories(
            Catch2
            INTERFACE
            ${CATCH_DIR}
    )
    set(CATCH_MAIN "${CMAKE_SOURCE_DIR}/test_main.cpp")
    add_compile_definitions(CATCH2=2)
  endif()
endif()

write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/strong_type/strong_type-config-version.cmake"
  VERSION ${STRONG_TYPE_VERSION}
  COMPATIBILITY AnyNewerVersion)

add_library(strong_type INTERFACE)

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

add_executable(
    self_test
  EXCLUDE_FROM_ALL
    test.cpp
    include/strong_type/strong_type.hpp
    ${CATCH_MAIN}
)

target_link_libraries(
    self_test
  PUBLIC
    Catch2::Catch2
    ${CATCHLIBS}
    strong_type
  )

add_subdirectory(test_fmt8)
add_subdirectory(test_fmt9)

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

