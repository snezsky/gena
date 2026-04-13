add_library(myproject
    "myproject.hpp"
    "myproject.cpp")
myproject_setup_target(myproject)

target_include_directories(myproject PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
