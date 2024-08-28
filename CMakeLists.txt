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

if(STRONG_TYPE_IMPORT_STD_LIBRARY)
  cmake_minimum_required(VERSION 3.30)
else()
  cmake_minimum_required(VERSION 3.14)
endif()

if(STRONG_TYPE_IMPORT_STD_LIBRARY)
  if("${CMAKE_GENERATOR}" STREQUAL "Visual Studio 17 2022")
    # This can be done before or after project() on MSVC
    # It works regardless of CMAKE_EXPERIMENTAL_CXX_IMPORT_STD
    # cmake -DSTRONG_TYPE_IMPORT_STD_LIBRARY=ON src_path
    set(CMAKE_CXX_SCAN_FOR_MODULES ON)
  else()
    # This needs to be done before selecting the languages so the project() command
    # The CMAKE_EXPERIMENTAL_CXX_IMPORT_STD is required
    # To build on calng you need a command line that uses Ninja points to a clang compiler and switched to use the clang libc++ not GCCs libstdc++
    # cmake -G Ninja -DCMAKE_CXX_FLAGS=-stdlib=libc++ -DSTRONG_TYPE_IMPORT_STD_LIBRARY=ON -DCMAKE_CXX_COMPILER=/usr/lib/llvm-20/bin/clang++ src_path
    set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "0e5b6991-d74f-4b3d-a41c-cf096e0b2508")
    set(CMAKE_CXX_MODULE_STD ON)
  endif()

  # We could do C++ 26 here as well, all compilers have agreed to support the std module in C++ 20 but CMake does not support that yet
  set(CMAKE_CXX_STANDARD 23)
endif()

project(strong_type)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

option(STRONG_TYPE_UNIT_TEST "Decide whether to build unit tests or not" OFF)

set(STRONG_TYPE_VERSION 15)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(MASTER_PROJECT OFF)
if (${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_SOURCE_DIR})
  set(MASTER_PROJECT ON)
endif()

  add_subdirectory(test)

if (${STRONG_TYPE_UNIT_TEST})

endif()

write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/strong_type/strong_type-config-version.cmake"
  VERSION ${STRONG_TYPE_VERSION}
  COMPATIBILITY AnyNewerVersion
  ARCH_INDEPENDENT)

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
