find_package(Qt6 REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()

add_executable(<@ project_name @>-app WIN32
    "main.cpp"
    "mainwindow.hpp"
    "mainwindow.cpp"
    "mainwindow.ui"
    "ico/app.rc") # windows icon
target_link_libraries(<@ project_name @>-app PRIVATE Qt6::Widgets <@ project_name @>)
<@ project_name @>_setup_target(<@ project_name @>-app)
