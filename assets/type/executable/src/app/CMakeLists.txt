add_executable(myproject-app "main.cpp")
target_link_libraries(myproject-app PRIVATE myproject)
myproject_setup_target(myproject-app)
