add_executable(example "example.cpp")
<@ lower(namespace) @>_setup_target(example)
target_link_libraries(example PRIVATE <@ project_name @>)
