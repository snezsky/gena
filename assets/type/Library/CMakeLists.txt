cmake_minimum_required(VERSION 3.28)

include("cmake/Setup.cmake")

project(<@ project_name @>)

option(<@ upper(namespace) @>_BUILD_TESTS     "build tests"     OFF)
option(<@ upper(namespace) @>_BUILD_EXAMPLES  "build examples"  OFF)

<% if dependencies != ["QTest"] %>
add_subdirectory("deps" SYSTEM EXCLUDE_FROM_ALL)
<% endif %>
add_subdirectory("src")

if (<@ upper(namespace) @>_BUILD_EXAMPLES)
    add_subdirectory("examples")
endif()

if (<@ upper(namespace) @>_BUILD_TESTS)
    enable_testing()
    add_subdirectory("tests")
endif()
