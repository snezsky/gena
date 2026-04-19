find_package(Qt6 REQUIRED COMPONENTS Widgets Concurrent)
qt_standard_project_setup()

add_executable(gena WIN32
    "main.cpp"
    "mainwindow.hpp"
    "mainwindow.cpp"
    "mainwindow.ui"
    "ico/app.rc") # windows icon
gena_setup_target(gena)
target_link_libraries(gena PRIVATE Qt6::Widgets Qt6::Concurrent generator)

message(STATUS "Copying project assets...")
file(COPY "${PROJECT_SOURCE_DIR}/assets" DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
set_target_properties(gena PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
