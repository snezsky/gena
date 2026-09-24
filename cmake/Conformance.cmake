function(__gena_target_enable_conformance TARGET_NAME)

    set_target_properties(${TARGET_NAME} PROPERTIES CXX_EXTENSIONS OFF)

    if (MSVC)
        target_compile_options(${TARGET_NAME} PRIVATE
            /Zc:__cplusplus   # report the real standard version in __cplusplus
            /Zc:preprocessor  # standard conforming preprocessor
            /utf-8)           # UTF-8 source and execution character sets
    endif()

endfunction()
