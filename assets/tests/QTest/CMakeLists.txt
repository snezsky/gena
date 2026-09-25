project(<@ project_name @>-tests)

find_package(Qt6 6.8 REQUIRED COMPONENTS Test)
qt_standard_project_setup() 

function(<@ lower(namespace) @>_add_test)

    cmake_parse_arguments(TEST "" "NAME" "SOURCES" ${ARGN})
    
    add_executable(${TEST_NAME} ${TEST_SOURCES})
    <@ lower(namespace) @>_setup_target(${TEST_NAME})

    target_link_libraries(${TEST_NAME} PRIVATE Qt6::Test)
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

    set_target_properties(${TEST_NAME} PROPERTIES FOLDER "tests")

endfunction()

<@ lower(namespace) @>_add_test(NAME test_<@ project_name @> SOURCES "test_<@ project_name @>.cpp")
<% if project_type == "QtQuickApplication" %>
target_link_libraries(test_<@ project_name @> PRIVATE <@ project_name @>-core)
<% else %>
target_link_libraries(test_<@ project_name @> PRIVATE <@ project_name @>)
<% endif %>

