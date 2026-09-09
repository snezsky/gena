# To avoid cluttering the target list
set(CMAKE_FOLDER "deps")

set(INJA_EXPORT OFF)
set(INJA_INSTALL OFF)
set(INJA_BUILD_TESTS OFF)
add_subdirectory("inja")

if (GENA_BUILD_TESTS)
    set(INSTALL_GTEST OFF)
    add_subdirectory("googletest")
endif()
