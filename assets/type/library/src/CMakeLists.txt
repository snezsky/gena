set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include/)

add_library(myproject
    "${INCLUDE_DIR}/myproject.hpp"
    "myproject.cpp")
myproject_setup_target(myproject)

target_include_directories(myproject PUBLIC ${INCLUDE_DIR})
