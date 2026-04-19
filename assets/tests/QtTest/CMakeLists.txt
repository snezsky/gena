project(myproject-tests)

find_package(Qt6 REQUIRED COMPONENTS Test)
qt_standard_project_setup() 

# name should be prefixed with `test_` for coverage
function(myproject_add_test)

    cmake_parse_arguments(TEST "" "NAME" "SOURCES" ${ARGN})
    
    add_executable(${TEST_NAME} ${TEST_SOURCES})
    myproject_setup_target(${TEST_NAME})

    target_link_libraries(${TEST_NAME} PRIVATE Qt6::Test)
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

    set_target_properties(${TEST_NAME} PROPERTIES FOLDER "tests")
    
    # clang-tidy hates QTest's macros
    set_source_files_properties(${TEST_SOURCES} PROPERTIES SKIP_LINTING ON)

endfunction()

myproject_add_test(NAME test_myproject SOURCES "test_myproject.cpp")
target_link_libraries(test_myproject PRIVATE myproject)

