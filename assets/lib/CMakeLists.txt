cmake_minimum_required(VERSION 3.28)

project(myproject)

include("cmake/setup.cmake")

option(MYPROJECT_BUILD_TESTS     "build tests"     ${MYPROJECT_ENABLE_DEVELOPER_MODE})
option(MYPROJECT_BUILD_EXAMPLES  "build examples"  ${MYPROJECT_ENABLE_DEVELOPER_MODE})

add_subdirectory("deps")
add_subdirectory("src")

if (MYPROJECT_BUILD_EXAMPLES)
    add_subdirectory("examples")
endif()

if (MYPROJECT_BUILD_TESTS)
    enable_testing()
    add_subdirectory("tests")
endif()
