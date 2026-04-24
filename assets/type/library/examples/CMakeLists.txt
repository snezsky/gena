add_executable(example "example.cpp")
<@ project_name @>_setup_target(example)
target_link_libraries(example PRIVATE <@ project_name @>)
