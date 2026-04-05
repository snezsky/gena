function(assert_target_property_equal TARGET PROPERTY EXPECTED)

    get_target_property(ACTUAL ${TARGET} ${PROPERTY})
    if (NOT "${ACTUAL}" STREQUAL "${EXPECTED}")
        message(FATAL_ERROR "Expected '${PROPERTY}' to be '${EXPECTED}', but got '${ACTUAL}'.")
    endif()

endfunction()

function(test_build_output NAME SOURCE REGEX)

    add_executable(${NAME}_exe EXCLUDE_FROM_ALL ${SOURCE})
    myproject_setup_target(${NAME}_exe)

    add_test(
        NAME ${NAME}
        COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target ${NAME}_exe)

    if (MYPROJECT_ENABLE_DEVELOPER_MODE)
        set_tests_properties(${NAME} PROPERTIES PASS_REGULAR_EXPRESSION ${REGEX})
    else()
        set_tests_properties(${NAME} PROPERTIES FAIL_REGULAR_EXPRESSION ${REGEX})
    endif()

endfunction()


function(test_program_output NAME SOURCE REGEX)

    add_executable(${NAME}_exe ${SOURCE})
    myproject_setup_target(${NAME}_exe)

    add_test(NAME ${NAME} COMMAND ${NAME}_exe)

    if (MYPROJECT_ENABLE_DEVELOPER_MODE)
        set_tests_properties(${NAME} PROPERTIES PASS_REGULAR_EXPRESSION ${REGEX})
    else()
        set_tests_properties(${NAME} PROPERTIES FAIL_REGULAR_EXPRESSION ${REGEX})
    endif()

endfunction()
