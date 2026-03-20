project(myproject-tests)

include("${myproject_SOURCE_DIR}/deps/Catch2/extras/Catch.cmake")

# expected prefix `test_` for coverage
# expected existing cpp file with the same name
function(myproject_add_test TARGET_NAME)

    add_executable(${TARGET_NAME} "${TARGET_NAME}.cpp")
    myproject_setup_target(${TARGET_NAME})

    target_link_libraries(${TARGET_NAME} PRIVATE Catch2::Catch2WithMain)
    catch_discover_tests(${TARGET_NAME})

    set_target_properties(${TARGET_NAME} PROPERTIES FOLDER "tests")

    #clang-tidy hates catch's macros
    get_target_property(CURRENT_SOURCES ${TARGET_NAME} SOURCES)
    set_source_files_properties(${CURRENT_SOURCES} PROPERTIES SKIP_LINTING ON)

endfunction()


myproject_add_test(test_myproject)
target_link_libraries(test_myproject PRIVATE myproject)

