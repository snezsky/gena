cmake_minimum_required(VERSION 3.25)

include("cmake/Setup.cmake")

project(<@ project_name @>)

option(<@ upper(namespace) @>_BUILD_TESTS     "build tests"     ${PROJECT_IS_TOP_LEVEL})
option(<@ upper(namespace) @>_BUILD_EXAMPLES  "build examples"  ${PROJECT_IS_TOP_LEVEL})

<% if has_submodules %>
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
