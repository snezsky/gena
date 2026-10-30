function(assert_process_succeeds)
    execute_process(
        COMMAND ${ARGV}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_TYPE}
        RESULT_VARIABLE PROCESS_RESULT
        OUTPUT_VARIABLE PROCESS_STDOUT
        ERROR_VARIABLE  PROCESS_STDERR)

    if (NOT ${PROCESS_RESULT} EQUAL 0)
        if (PROCESS_STDERR)
            message(STATUS "Error output: ${PROCESS_STDERR}")
        endif()
        if (PROCESS_STDOUT)
            message(STATUS "Standard output: ${PROCESS_STDOUT}")
        endif()
        message(FATAL_ERROR "Stopping CMake due to failed process ( ${ARGV} )") 
    endif()

endfunction()

set(PRESETS android-release android-debug)
if(DEFINED ENV{VisualStudioVersion})
    # Can't check msvc conventional way in the script mode
    list(APPEND PRESETS msvc-debug msvc-release)
else()
    list(APPEND PRESETS clang-debug clang-release gcc-debug gcc-release)
endif()

foreach (PRESET ${PRESETS})
    assert_process_succeeds(${CMAKE_COMMAND} --workflow --preset ${PRESET})
    assert_process_succeeds(${CMAKE_COMMAND} --install out/build/${PRESET})
endforeach()
