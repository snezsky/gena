find_package(Qt6 6.4.2 REQUIRED COMPONENTS Quick)
qt_standard_project_setup()

qt_add_executable(<@ project_name @> WIN32
    "main.cpp"
    "resources/<@ project_name @>.rc"
    "resources/<@ project_name @>.qrc"
)
qt_add_qml_module(<@ project_name @>
    URI <@ capitalize(project_name) @>.App
    VERSION 0.1
    RESOURCE_PREFIX "/"
    SOURCES
        "main.cpp"
    QML_FILES
        "Main.qml"
)

set_target_properties(<@ project_name @> PROPERTIES AUTORCC ON)
target_link_libraries(<@ project_name @> PRIVATE Qt6::Quick core coreplugin)

install(TARGETS <@ project_name @> RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
qt_generate_deploy_qml_app_script(
    TARGET <@ project_name @>
    OUTPUT_SCRIPT <@ project_name @>_deploy
    NO_TRANSLATIONS
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${<@ project_name @>_deploy})
