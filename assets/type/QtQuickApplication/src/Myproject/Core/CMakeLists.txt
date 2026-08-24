find_package(Qt6 6.4.2 REQUIRED COMPONENTS Quick)
qt_standard_project_setup()

qt_add_qml_module(core
    URI     <@ capitalize(project_name) @>.Core
    VERSION 0.1
    STATIC
    RESOURCE_PREFIX "/"
    SOURCES
        "include/<@ project_name @>/<@ project_name @>.hpp"
        "<@ project_name @>.cpp"
)

target_link_libraries(core PUBLIC Qt6::Qml)
target_include_directories(core 
    PUBLIC  "${CMAKE_CURRENT_SOURCE_DIR}/include"
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/<@ project_name @>"
)
