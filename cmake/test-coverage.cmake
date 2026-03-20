function(__gena_target_enable_test_coverage TARGET_NAME)

    if (NOT GENA_ENABLE_TEST_COVERAGE)
        return()
    endif()

    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        target_link_libraries(${TARGET_NAME} PRIVATE --coverage)
        target_compile_options(${TARGET_NAME} PRIVATE --coverage -g)
    endif()
    if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        target_compile_options(${TARGET_NAME} PRIVATE -fprofile-instr-generate)
        target_link_options(${TARGET_NAME} PRIVATE -fprofile-instr-generate -fcoverage-mapping)
    endif()

endfunction()
