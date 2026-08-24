cmake_minimum_required(VERSION 3.28)

include("cmake/Setup.cmake")

project(
  gena
  DESCRIPTION "C++ project generator"
  HOMEPAGE_URL "https://github.com/snezsky/gena"
  LANGUAGES CXX)
  
option(GENA_BUILD_TESTS "build tests" OFF)

add_subdirectory("deps" SYSTEM)
add_subdirectory("src/Generator")
add_subdirectory("src/Gena")

if (GENA_BUILD_TESTS)
    enable_testing()
    add_subdirectory("tests/unit")
    add_subdirectory("tests/integration")
endif()
