
function(<@ lower(namespace) @>_add_example)

    cmake_parse_arguments(EXAMPLE "" "NAME" "SOURCES" ${ARGN})

    add_executable(${EXAMPLE_NAME} ${EXAMPLE_SOURCES})
    target_link_libraries(${EXAMPLE_NAME} PRIVATE <@ project_name @>)
    set_target_properties(${EXAMPLE_NAME} PROPERTIES FOLDER "examples")

    <@ lower(namespace) @>_setup_target(${EXAMPLE_NAME})

endfunction()

<@ lower(namespace) @>_add_example(NAME example SOURCES "example.cpp")
