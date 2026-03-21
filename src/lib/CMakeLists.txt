find_package(Qt6 REQUIRED COMPONENTS Core Gui)

add_library(generator
    "options.hpp"
    "replacements.hpp"
    "replacements.cpp"
    "generator.hpp"
    "generator.cpp")
gena_setup_target(generator)

target_link_libraries(generator PUBLIC Qt6::Core)
target_include_directories(generator PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
