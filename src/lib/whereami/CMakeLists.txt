# Modernized C++ wrapper for the popular 'whereami' library
# The original project is linked in the description.

project(
  whereami
  DESCRIPTION "Utility for obtainging executable location"
  HOMEPAGE_URL "https://github.com/gpakosz/whereami"
  LANGUAGES C CXX)

add_library(whereami
  "include/whereami/whereami.h"
  "include/whereami/whereami.hpp"
  "whereami.c"
  "whereami.cpp")

target_include_directories(whereami
  PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

set_target_properties(whereami
  PROPERTIES CXX_STANDARD 17)
