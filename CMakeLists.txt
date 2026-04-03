cmake_minimum_required(VERSION 3.28)

project(
  gena
  DESCRIPTION "C++ project generator"
  HOMEPAGE_URL "https://github.com/snezsky/gena"
  LANGUAGES CXX)

include("cmake/Setup.cmake")

add_subdirectory("src/lib")
add_subdirectory("src/app")
