project(myproject-tests)

include("GoogleTest")

# name should be prefixed with `test_` for coverage
function(myproject_add_test)

    cmake_parse_arguments(TEST "" "NAME" "SOURCES" ${ARGN})
    
    add_executable(${TEST_NAME} ${TEST_SOURCES})
    myproject_setup_target(${TEST_NAME})

    target_link_libraries(${TEST_NAME} PRIVATE GTest::gtest_main)
    gtest_discover_tests(${TEST_NAME})

    set_target_properties(${TEST_NAME} PROPERTIES FOLDER "tests")
    
    # clang-tidy hates googletest's macros
    set_source_files_properties(${TEST_SOURCES} PROPERTIES SKIP_LINTING ON)

endfunction()


myproject_add_test(NAME test_myproject SOURCES "test_myproject.cpp")
target_link_libraries(test_myproject PRIVATE myproject)

