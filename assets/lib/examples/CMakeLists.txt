add_executable(example "example.cpp")
myproject_setup_target(example)
target_link_libraries(example PRIVATE myproject)
