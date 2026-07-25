cmake_minimum_required(VERSION 3.28)

project(<@ project_name @>)

include("cmake/Setup.cmake")

option(<@ upper(namespace) @>_BUILD_TESTS     "build tests"     OFF)
option(<@ upper(namespace) @>_BUILD_EXAMPLES  "build examples"  OFF)

add_subdirectory("deps" SYSTEM EXCLUDE_FROM_ALL)
add_subdirectory("src")

if (<@ upper(namespace) @>_BUILD_EXAMPLES)
    add_subdirectory("examples")
endif()

if (<@ upper(namespace) @>_BUILD_TESTS)
    enable_testing()
    add_subdirectory("tests")
endif()
