find_package(Qt6 6.4.2 REQUIRED COMPONENTS Core Qml)

add_subdirectory(whereami)

add_library(generator
    "git_client.hpp"
    "git_client.cpp"
    "generation_options.hpp"
    "generation_options_to_strings.hpp"
    "string_extensions.hpp"
    "rendering_options.hpp"
    "options_validator.hpp"
    "options_validator.cpp"
    "file_editor.hpp"
    "file_editor.cpp"
    "generator.hpp"
    "generator.cpp")
gena_setup_target(generator)

target_link_libraries(generator
    PUBLIC  Qt6::Core
    PRIVATE pantor::inja whereami)
target_include_directories(generator PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
