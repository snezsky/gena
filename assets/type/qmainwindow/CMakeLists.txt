cmake_minimum_required(VERSION 3.28)

project(<@ project_name @>)

include("cmake/Setup.cmake")

option(<@ upper(project_name) @>_BUILD_TESTS "build tests" OFF)

add_subdirectory("deps" EXCLUDE_FROM_ALL)
add_subdirectory("src/app")
add_subdirectory("src/lib")

if (<@ upper(project_name) @>_BUILD_TESTS)
    enable_testing()
    add_subdirectory("tests")
endif()
