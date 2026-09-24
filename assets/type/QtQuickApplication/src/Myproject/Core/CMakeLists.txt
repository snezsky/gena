find_package(Qt6 6.8 REQUIRED COMPONENTS Quick)
qt_standard_project_setup(REQUIRES 6.8)

qt_add_qml_module(<@ project_name @>-core
    URI     <@ capitalized_name @>.Core
    VERSION 0.1
    STATIC
    SOURCES
        "include/<@ project_name @>/<@ project_name @>.hpp"
        "<@ project_name @>.cpp"
)

target_link_libraries(<@ project_name @>-core PUBLIC Qt6::Qml)
target_include_directories(<@ project_name @>-core
    PUBLIC  "${CMAKE_CURRENT_SOURCE_DIR}/include"
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/<@ project_name @>"
)
