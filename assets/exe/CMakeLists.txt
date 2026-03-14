cmake_minimum_required(VERSION 3.28)

project(myproject)

include("cmake/setup.cmake")

add_subdirectory("deps")
add_subdirectory("src/app")
add_subdirectory("src/lib")
add_subdirectory("tests")
