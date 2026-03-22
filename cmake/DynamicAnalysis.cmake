function(__gena_target_enable_dynamic_analysis TARGET_NAME)

    if (NOT GENA_ENABLE_DYNAMIC_ANALYSIS)
        return()
    endif()
    
    if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_link_options(${TARGET_NAME} PRIVATE -fsanitize=address,undefined)
        target_compile_options(${TARGET_NAME} PRIVATE -fsanitize=address,undefined)
    endif()
    
    if (MSVC)
        target_link_options(${TARGET_NAME} PRIVATE /INCREMENTAL:NO)
        target_compile_options(${TARGET_NAME} PRIVATE /fsanitize=address /Zi)
        target_compile_definitions(${TARGET_NAME} PRIVATE _DISABLE_VECTOR_ANNOTATION _DISABLE_STRING_ANNOTATION)
    endif()

endfunction()