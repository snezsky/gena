option(GENA_ENABLE_WARNINGS          "more warnings and werror"  ${GENA_ENABLE_DEVELOPER_MODE})
option(GENA_ENABLE_TEST_COVERAGE     "test coverage"             ${GENA_ENABLE_DEVELOPER_MODE})
option(GENA_ENABLE_STATIC_ANALYSIS   "clang-tidy, cppcheck"      ${GENA_ENABLE_DEVELOPER_MODE})  
option(GENA_ENABLE_DYNAMIC_ANALYSIS  "available sanitizers"      ${GENA_ENABLE_DEVELOPER_MODE})

include("cmake/Cache.cmake")
include("cmake/Warnings.cmake")
include("cmake/Hardening.cmake")
include("cmake/NiceOutput.cmake")
include("cmake/TestCoverage.cmake")
include("cmake/StaticAnalysis.cmake")
include("cmake/DynamicAnalysis.cmake")

function(gena_setup_target TARGET_NAME)

    set_target_properties(
        ${TARGET_NAME} PROPERTIES
        CXX_STANDARD            20
        CXX_STANDARD_REQUIRED   ON
        EXPORT_COMPILE_COMMANDS ON
        CXX_EXTENSIONS          OFF)
    
    __gena_target_enable_cache(${TARGET_NAME})
    __gena_target_enable_warnings(${TARGET_NAME})
    __gena_target_enable_hardening(${TARGET_NAME})
    __gena_target_enable_nice_output(${TARGET_NAME})
    __gena_target_enable_test_coverage(${TARGET_NAME})
    __gena_target_enable_static_analysis(${TARGET_NAME})
    __gena_target_enable_dynamic_analysis(${TARGET_NAME})

endfunction()

function(gena_run_windeployqt TARGET_NAME)

    if (NOT MSVC)
        return()
    endif()

    add_custom_command(
        TARGET ${TARGET_NAME} POST_BUILD
        COMMAND ${WINDEPLOYQT_EXECUTABLE} $<TARGET_FILE:${TARGET_NAME}> --no-translations
        COMMENT "Deploying Qt runtime dependencies for ${TARGET_NAME}")

endfunction()
