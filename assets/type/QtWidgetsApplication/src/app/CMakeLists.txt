find_package(Qt6 6.4.2 REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()

add_executable(<@ project_name @>-app WIN32
    "main.cpp"
    "mainwindow.hpp"
    "mainwindow.cpp"
    "mainwindow.ui"
    "ico/app.rc") # windows icon
target_link_libraries(<@ project_name @>-app PRIVATE Qt6::Widgets <@ project_name @>)
<@ lower(namespace) @>_setup_target(<@ project_name @>-app)


install(TARGETS <@ project_name @>-app RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
qt_generate_deploy_app_script(
    TARGET <@ project_name @>-app
    OUTPUT_SCRIPT <@ project_name @>_deploy
    NO_TRANSLATIONS
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${<@ project_name @>_deploy})
