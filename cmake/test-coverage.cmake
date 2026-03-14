function(__gena_target_enable_test_coverage TARGET_NAME)

    if (NOT GENA_ENABLE_TEST_COVERAGE)
        return()
    endif()

    if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_link_libraries(${TARGET_NAME} PRIVATE --coverage)
        target_compile_options(${TARGET_NAME} PRIVATE --coverage -g)
    endif()

endfunction()
