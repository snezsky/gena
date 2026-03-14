include(CheckCXXCompilerFlag)

function(__gena_target_enable_hardening TARGET_NAME)

    if(MSVC)
        target_link_options(${TARGET_NAME} PRIVATE /NXCOMPAT /CETCOMPAT)
        target_compile_options(${TARGET_NAME} PRIVATE /sdl /DYNAMICBASE /guard:cf)
    endif()

    if(CMAKE_CXX_COMPILER_ID MATCHES ".*Clang|GNU")
        check_cxx_compiler_flag(-fcf-protection CF_PROTECTION)
        check_cxx_compiler_flag(-fstack-protector-strong STACK_PROTECTOR)
        check_cxx_compiler_flag(-fstack-clash-protection CLASH_PROTECTION)

        target_compile_options(
            ${TARGET_NAME} PRIVATE
            $<$<BOOL:CF_PROTECTION>:-fcf-protection>
            $<$<BOOL:STACK_PROTECTOR>:-fstack-protector-strong>
            $<$<BOOL:CLASH_PROTECTION>:-fstack-clash-protection>
            $<$<NOT:$<CONFIG:Debug>>:-U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=3>)

        target_compile_definitions(${TARGET_NAME} PRIVATE -D_GLIBCXX_ASSERTIONS)
    endif()

endfunction()