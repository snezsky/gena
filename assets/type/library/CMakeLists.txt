cmake_minimum_required(VERSION 3.28)

project(myproject)

include("cmake/Setup.cmake")

option(MYPROJECT_BUILD_TESTS     "build tests"     OFF)
option(MYPROJECT_BUILD_EXAMPLES  "build examples"   ON)

add_subdirectory("deps" EXCLUDE_FROM_ALL)
add_subdirectory("src")

if (MYPROJECT_BUILD_EXAMPLES)
    add_subdirectory("examples")
endif()

if (MYPROJECT_BUILD_TESTS)
    enable_testing()
    add_subdirectory("tests")
endif()
