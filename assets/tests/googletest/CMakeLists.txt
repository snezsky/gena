project(<@ project_name @>-tests)

include("GoogleTest")

# name should be prefixed with `test_` for coverage
function(<@ lower(namespace) @>_add_test)

    cmake_parse_arguments(TEST "" "NAME" "SOURCES" ${ARGN})
    
    add_executable(${TEST_NAME} ${TEST_SOURCES})
    <@ lower(namespace) @>_setup_target(${TEST_NAME})

    target_link_libraries(${TEST_NAME} PRIVATE GTest::gtest_main)
    gtest_discover_tests(${TEST_NAME})

    set_target_properties(${TEST_NAME} PROPERTIES FOLDER "tests")
    
    # clang-tidy hates googletest's macros
    set_source_files_properties(${TEST_SOURCES} PROPERTIES SKIP_LINTING ON)

endfunction()


<@ lower(namespace) @>_add_test(NAME test_<@ project_name @> SOURCES "test_<@ project_name @>.cpp")
target_link_libraries(test_<@ project_name @> PRIVATE <@ project_name @>)

