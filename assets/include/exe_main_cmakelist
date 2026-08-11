cmake_minimum_required(VERSION 3.28)

project(<@ project_name @>)

include("cmake/Setup.cmake")

option(<@ upper(namespace) @>_BUILD_TESTS "build tests" OFF)

<% if length(dependencies) > 1 or dependencies.0 != "QTest" %>
add_subdirectory("deps" SYSTEM EXCLUDE_FROM_ALL)
<% endif %>
<% if project_type == "QtQuickApplication" %>
add_subdirectory("src/<@ capitalize(project_name) @>/Core")
add_subdirectory("src/<@ capitalize(project_name) @>/App")
<% else %>
add_subdirectory("src/app")
add_subdirectory("src/lib")
<% endif %>

if (<@ upper(namespace) @>_BUILD_TESTS)
    enable_testing()
    add_subdirectory("tests")
endif()
