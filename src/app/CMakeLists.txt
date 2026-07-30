find_package(Qt6 REQUIRED COMPONENTS Quick)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)

qt_add_executable(gena WIN32
    "main.cpp"
    "resources/gena.rc"
    "resources/gena.qrc"
)
qt_add_qml_module(gena
    URI             Gena
    VERSION         0.1
    QML_FILES
        "Main.qml"
        "MessageBox.qml"
        "LabeledComboBox.qml"
        "LabeledTextField.qml"
)
target_link_libraries(gena PRIVATE Qt6::Quick Qt6::Qml generator generatorplugin)
gena_setup_target(gena)

message(STATUS "Copying assets for gena...")
file(COPY "${PROJECT_SOURCE_DIR}/assets" DESTINATION ${CMAKE_CURRENT_BINARY_DIR})