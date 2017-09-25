cmake_minimum_required(VERSION 3.7)
project(strong_type)

set(CMAKE_CXX_STANDARD 14)
set(SOURCE_FILES test.cpp strong_type.hpp)

set(MASTER_PROJECT OFF)
if (${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_SOURCE_DIR})
  set(MASTER_PROJECT ON)
endif()

if (MASTER_PROJECT AND CMAKE_BUILD_TYPE MATCHES Debug)
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

add_executable(
    self_test
  EXCLUDE_FROM_ALL
    test.cpp
    )
target_include_directories(
    self_test
  PRIVATE
    ${CATCH_DIR}
)

#install(
#  FILES
#    strong_type.hpp
#  DESTINATION
#    ${CMAKE_INSTALL_INCLUDEDIR}
#)

