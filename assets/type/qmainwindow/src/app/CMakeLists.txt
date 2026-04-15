find_package(Qt6 REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()

add_executable(myproject-app WIN32
    "main.cpp"
    "mainwindow.hpp"
    "mainwindow.cpp"
    "mainwindow.ui"
    "ico/app.rc") # windows icon
target_link_libraries(myproject-app PRIVATE Qt6::Widgets myproject)
myproject_setup_target(myproject-app)

if (MSVC)
    add_custom_command(
        TARGET myproject-app POST_BUILD
        COMMAND ${WINDEPLOYQT_EXECUTABLE} $<TARGET_FILE:myproject-app> --no-translations)
endif()
