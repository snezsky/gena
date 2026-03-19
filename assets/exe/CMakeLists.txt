cmake_minimum_required(VERSION 3.28)

project(myproject)

include("cmake/setup.cmake")

option(MYPROJECT_BUILD_TESTS "build tests" ${MYPROJECT_ENABLE_DEVELOPER_MODE})

add_subdirectory("deps")
add_subdirectory("src/app")
add_subdirectory("src/lib")

if (MYPROJECT_BUILD_TESTS)
    add_subdirectory("tests")
endif()