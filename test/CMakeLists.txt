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
  set(CATCH_MAIN "${CMAKE_SOURCE_DIR}/test/test_main.cpp")
  add_compile_definitions(CATCH2=2)
endif()

add_executable(
        self_test
        EXCLUDE_FROM_ALL
        test.cpp
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
