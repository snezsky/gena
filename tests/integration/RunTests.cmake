function(assert_process_succeeds)
    execute_process(
        COMMAND ${ARGV}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${GEN_NAME}
        RESULT_VARIABLE PROCESS_RESULT
        OUTPUT_VARIABLE PROCESS_STDOUT
        ERROR_VARIABLE  PROCESS_STDERR)

    if (NOT ${PROCESS_RESULT} EQUAL 0)
        message(STATUS "Error output: ${PROCESS_STDERR}")
        message(STATUS "Standard output: ${PROCESS_STDOUT}")
        message(FATAL_ERROR "Stopping CMake due to failed process ( ${ARGV} )")
    endif()

endfunction()

# Can't check msvc conventional way in the script mode
if(DEFINED ENV{VisualStudioVersion})
    set(PRESETS msvc-debug-dev msvc-release-user)
else()
    set(PRESETS clang-debug-dev clang-release-user gcc-debug-dev gcc-release-user)
endif()

string(TOUPPER ${GEN_NAME} GEN_NAME_UPPER)

foreach (PRESET ${PRESETS})
    assert_process_succeeds(${CMAKE_COMMAND} --preset ${PRESET} -D${GEN_NAME_UPPER}_BUILD_TESTS=ON)
    assert_process_succeeds(${CMAKE_COMMAND} --build build/${PRESET})
    assert_process_succeeds(${CMAKE_CTEST_COMMAND} --preset test-${PRESET})
endforeach()
