option(<@ upper(namespace) @>_ENABLE_WARNINGS          "more warnings and werror"  ${<@ upper(namespace) @>_ENABLE_DEVELOPER_MODE})
option(<@ upper(namespace) @>_ENABLE_TEST_COVERAGE     "test coverage"             ${<@ upper(namespace) @>_ENABLE_DEVELOPER_MODE})
option(<@ upper(namespace) @>_ENABLE_STATIC_ANALYSIS   "clang-tidy, cppcheck"      ${<@ upper(namespace) @>_ENABLE_DEVELOPER_MODE})
option(<@ upper(namespace) @>_ENABLE_DYNAMIC_ANALYSIS  "available sanitizers"      ${<@ upper(namespace) @>_ENABLE_DEVELOPER_MODE})

include("cmake/Cache.cmake")
include("cmake/Warnings.cmake")
include("cmake/Hardening.cmake")
include("cmake/NiceOutput.cmake")
include("cmake/TestCoverage.cmake")
include("cmake/StaticAnalysis.cmake")
include("cmake/DynamicAnalysis.cmake")

function(<@ lower(namespace) @>_setup_target TARGET_NAME)

    set_target_properties(
        ${TARGET_NAME} PROPERTIES
        CXX_EXTENSIONS          OFF
        CXX_SCAN_FOR_MODULES    OFF
        CXX_STANDARD            <@ cpp_standard @>
        CXX_STANDARD_REQUIRED   ON
        EXPORT_COMPILE_COMMANDS ON)
    
    __<@ lower(namespace) @>_target_enable_cache(${TARGET_NAME})
    __<@ lower(namespace) @>_target_enable_warnings(${TARGET_NAME})
    __<@ lower(namespace) @>_target_enable_hardening(${TARGET_NAME})
    __<@ lower(namespace) @>_target_enable_nice_output(${TARGET_NAME})
    __<@ lower(namespace) @>_target_enable_test_coverage(${TARGET_NAME})
    __<@ lower(namespace) @>_target_enable_static_analysis(${TARGET_NAME})
    __<@ lower(namespace) @>_target_enable_dynamic_analysis(${TARGET_NAME})

endfunction()
