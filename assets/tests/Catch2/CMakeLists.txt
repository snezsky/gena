project(<@ project_name @>-tests)

include("${<@ project_name @>_SOURCE_DIR}/deps/Catch2/extras/Catch.cmake")

# name should be prefixed with `test_` for coverage
function(<@ project_name @>_add_test)

    cmake_parse_arguments(TEST "" "NAME" "SOURCES" ${ARGN})
    
    add_executable(${TEST_NAME} ${TEST_SOURCES})
    <@ project_name @>_setup_target(${TEST_NAME})

    target_link_libraries(${TEST_NAME} PRIVATE Catch2::Catch2WithMain)
    catch_discover_tests(${TEST_NAME})

    set_target_properties(${TEST_NAME} PROPERTIES FOLDER "tests")
    
    # clang-tidy hates Catch's macros
    set_source_files_properties(${TEST_SOURCES} PROPERTIES SKIP_LINTING ON)

endfunction()


<@ project_name @>_add_test(NAME test_<@ project_name @> SOURCES "test_<@ project_name @>.cpp")
target_link_libraries(test_<@ project_name @> PRIVATE <@ project_name @>)

