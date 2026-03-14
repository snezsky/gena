cmake_minimum_required(VERSION 3.28)

project(
  gena
  DESCRIPTION "C++ project generator"
  LANGUAGES CXX)

include("cmake/setup.cmake")

#TODO: make it part of my setup
find_package(Qt6 REQUIRED COMPONENTS Core)
qt_standard_project_setup()

add_subdirectory("src/lib")
add_subdirectory("src/app")
