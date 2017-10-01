cmake_minimum_required(VERSION 3.7)
project(strong_type)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

set(STRONG_TYPE_VERSION 0.0.1)
set(SOURCE_FILES test.cpp include/strong_type.hpp)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(MASTER_PROJECT OFF)
if (${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_SOURCE_DIR})
  set(MASTER_PROJECT ON)
endif()

if (MASTER_PROJECT AND CMAKE_BUILD_TYPE MATCHES Debug)

  set(CMAKE_CXX_STANDARD_REQUIRED YES)
  set(CMAKE_CXX_EXTENSIONS OFF)

  set(CATCH_DIR ${CMAKE_CURRENT_BINARY_DIR}/catch)
  if(NOT EXISTS ${CATCH_DIR}/catch.hpp)
    if (NOT EXISTS ${CATCH_DIR})
      make_directory(${CATCH_DIR})
    endif()
    file(
      DOWNLOAD
        https://raw.githubusercontent.com/philsquared/Catch/master/single_include/catch.hpp  ${CATCH_DIR}/catch.hpp
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
    include/strong_type.hpp
    )
target_include_directories(
    self_test
  PRIVATE
    ${CATCH_DIR}
)
target_link_libraries(
    self_test
  PRIVATE
    strong_type
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
  FILES
    include/strong_type.hpp
  DESTINATION
    ${CMAKE_INSTALL_INCLUDEDIR}
)

