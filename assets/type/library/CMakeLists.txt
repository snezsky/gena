cmake_minimum_required(VERSION 3.28)

project(<@ project_name @>)

include("cmake/Setup.cmake")

option(<@ upper(project_name) @>_BUILD_TESTS     "build tests"     OFF)
option(<@ upper(project_name) @>_BUILD_EXAMPLES  "build examples"   ON)

add_subdirectory("deps" EXCLUDE_FROM_ALL)
add_subdirectory("src")

if (<@ upper(project_name) @>_BUILD_EXAMPLES)
    add_subdirectory("examples")
endif()

if (<@ upper(project_name) @>_BUILD_TESTS)
    enable_testing()
    add_subdirectory("tests")
endif()
