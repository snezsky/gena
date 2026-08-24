find_package(Qt6 6.4.2 REQUIRED COMPONENTS Quick)
qt_standard_project_setup()
set(CMAKE_AUTORCC ON)

qt_add_library(gena-qml STATIC "resources/gena.qrc")
qt_add_qml_module(gena-qml
    URI       Gena
    VERSION   0.1
    RESOURCE_PREFIX "/"
    SOURCES
        "qgenerator.hpp"
        "qgenerator.cpp"
        "../Generator/generation_options.hpp"
    QML_FILES
        "Main.qml"
        "MessageBox.qml"
        "LabeledComboBox.qml"
        "LabeledTextField.qml"
)
target_link_libraries(gena-qml PUBLIC Qt6::Quick Qt6::Qml generator)

qt_add_executable(gena WIN32
    "main.cpp"
    "resources/gena.rc"
)
target_link_libraries(gena PRIVATE gena-qml gena-qmlplugin)

message(STATUS "Copying assets for gena...")
file(COPY "${PROJECT_SOURCE_DIR}/assets" DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# otherwise linters will try to analyze generated code
set(GENA_ENABLE_WARNINGS OFF)
set(GENA_ENABLE_STATIC_ANALYSIS OFF)
gena_setup_target(gena)
gena_setup_target(gena-qml)

qt_generate_deploy_qml_app_script(
    TARGET gena
    OUTPUT_SCRIPT qml_deploy
    NO_TRANSLATIONS
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(TARGETS gena)
install(SCRIPT ${qml_deploy})
install(DIRECTORY "${PROJECT_SOURCE_DIR}/assets" TYPE BIN)
