# reusing googletest from generator's assets
set(CMAKE_FOLDER "tests/unit/deps")
add_subdirectory(
    "${PROJECT_SOURCE_DIR}/assets/deps/googletest"
    "${PROJECT_BINARY_DIR}/tests/googletest" EXCLUDE_FROM_ALL)
include("GoogleTest")
set(CMAKE_FOLDER "tests/unit")

function(gena_add_unit_test)

    cmake_parse_arguments(TEST "" "NAME" "SOURCES" ${ARGN})
    
    add_executable(${TEST_NAME} ${TEST_SOURCES})
    gena_setup_target(${TEST_NAME})

    target_link_libraries(${TEST_NAME} PRIVATE generator GTest::gtest_main)
    gtest_discover_tests(${TEST_NAME} DISCOVERY_MODE PRE_TEST)

    # clang-tidy hates googletest's macros
    set_source_files_properties(${TEST_SOURCES} PROPERTIES SKIP_LINTING ON)

    if (MSVC)
        add_custom_command(
            TARGET ${TEST_NAME} POST_BUILD
            COMMAND ${WINDEPLOYQT_EXECUTABLE} $<TARGET_FILE:${TEST_NAME}> --no-translations)
    endif()

endfunction()

gena_add_unit_test(NAME test_options_validator SOURCES "test_options_validator.cpp")
