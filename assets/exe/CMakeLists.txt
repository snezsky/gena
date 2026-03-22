cmake_minimum_required(VERSION 3.28)

project(myproject)

include("cmake/setup.cmake")

option(MYPROJECT_BUILD_TESTS "build tests" ${MYPROJECT_ENABLE_DEVELOPER_MODE})

add_subdirectory("deps" EXCLUDE_FROM_ALL)
add_subdirectory("src/app")
add_subdirectory("src/lib")

if (MYPROJECT_BUILD_TESTS)
    enable_testing()
    add_subdirectory("tests")
endif()