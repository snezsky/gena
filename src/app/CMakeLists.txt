find_package(Qt6 REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()

add_executable(gena WIN32
    "main.cpp"
    "mainwindow.hpp"
    "mainwindow.cpp"
    "mainwindow.ui"
    "ico/app.rc") # windows icon
target_link_libraries(gena PRIVATE Qt6::Widgets generator)
gena_setup_target(gena)

message(STATUS "Copying project assets...")
file(COPY "${PROJECT_SOURCE_DIR}/assets" DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
set_target_properties(gena PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
