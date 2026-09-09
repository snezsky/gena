include("GoogleTest")

function(gena_add_unit_test)

    cmake_parse_arguments(TEST "" "NAME" "SOURCES" ${ARGN})

    list(APPEND TEST_SOURCES "auxiliary.hpp")
    add_executable(${TEST_NAME} ${TEST_SOURCES})
    gena_setup_target(${TEST_NAME})

    target_link_libraries(${TEST_NAME} PRIVATE generator GTest::gtest_main GTest::gmock)
    gtest_discover_tests(${TEST_NAME})

    set_target_properties(${TEST_NAME} PROPERTIES FOLDER "tests/unit")
    set_source_files_properties(${TEST_SOURCES} PROPERTIES SKIP_LINTING ON)

endfunction()

gena_add_unit_test(NAME test_generator SOURCES "test_generator.cpp")
gena_add_unit_test(NAME test_file_editor SOURCES "test_file_editor.cpp")
gena_add_unit_test(NAME test_options_validator SOURCES "test_options_validator.cpp")

message(STATUS "Copying assets for unit tests...")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/test_data" DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

