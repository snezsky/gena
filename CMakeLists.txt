cmake_minimum_required(VERSION 3.28)

project(
  gena
  DESCRIPTION "C++ project generator"
  LANGUAGES CXX)

include("cmake/setup.cmake")

add_subdirectory("src/lib")
add_subdirectory("src/app")
