cmake_minimum_required(VERSION 3.28)

project(
  gena
  DESCRIPTION "C++ project generator"
  HOMEPAGE_URL "https://github.com/snezsky/gena"
  LANGUAGES CXX)

include("cmake/Setup.cmake")

option(GENA_BUILD_TESTS "build tests" OFF)

add_subdirectory("src/lib")
add_subdirectory("src/app")

if (GENA_BUILD_TESTS)
    enable_testing()
    add_subdirectory("tests/unit")
    add_subdirectory("tests/integration")
endif()
