add_executable(<@ project_name @>-app "main.cpp")
target_link_libraries(<@ project_name @>-app PRIVATE <@ project_name @>)
<@ lower(namespace) @>_setup_target(<@ project_name @>-app)

include(GNUInstallDirs)
install(TARGETS <@ project_name @>-app RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
