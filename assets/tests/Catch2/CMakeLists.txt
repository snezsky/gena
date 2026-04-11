project(myproject-tests)

include("${myproject_SOURCE_DIR}/deps/Catch2/extras/Catch.cmake")

# name should be prefixed with `test_` for coverage
function(myproject_add_test)

    cmake_parse_arguments(TEST "" "NAME" "SOURCES" ${ARGN})
    
    add_executable(${TEST_NAME} ${TEST_SOURCES})
    myproject_setup_target(${TEST_NAME})

    target_link_libraries(${TEST_NAME} PRIVATE Catch2::Catch2WithMain)
    catch_discover_tests(${TEST_NAME} DISCOVERY_MODE PRE_TEST)

    set_target_properties(${TEST_NAME} PROPERTIES FOLDER "tests")
    
    # clang-tidy hates Catch's macros
    set_source_files_properties(${TEST_SOURCES} PROPERTIES SKIP_LINTING ON)

endfunction()


myproject_add_test(NAME test_myproject SOURCES "test_myproject.cpp")
target_link_libraries(test_myproject PRIVATE myproject)

