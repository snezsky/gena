include(CheckLinkerFlag)
include(CheckCXXCompilerFlag)

function(__<@ lower(namespace) @>_target_enable_hardening TARGET_NAME)

    if(MSVC)
        target_compile_options(${TARGET_NAME} PRIVATE
            /guard:cf   # emit Control Flow Guard checks before indirect calls
            /sdl)       # unsafe CRT calls are errors, deleted pointers poisoned, members zero-initialized

        target_link_options(${TARGET_NAME} PRIVATE
            /guard:cf   # mark the image as CFG-enabled so Windows enforces the checks
            /CETCOMPAT) # opt into hardware shadow stacks (CET) protecting return addresses
    endif()

    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        check_cxx_compiler_flag(-fcf-protection <@ upper(namespace) @>_HAS_CF_PROTECTION)
        check_cxx_compiler_flag(-fstack-protector-strong <@ upper(namespace) @>_HAS_STACK_PROTECTOR)
        check_cxx_compiler_flag(-fstack-clash-protection <@ upper(namespace) @>_HAS_CLASH_PROTECTION)
        check_linker_flag(CXX "LINKER:-z,relro,-z,now" <@ upper(namespace) @>_HAS_FULL_RELRO)
        check_linker_flag(CXX "LINKER:-z,noexecstack" <@ upper(namespace) @>_HAS_NOEXECSTACK)

        target_compile_options(${TARGET_NAME} PRIVATE
            # calls through pointers can't be redirected to arbitrary code
            $<$<BOOL:${<@ upper(namespace) @>_HAS_CF_PROTECTION}>:-fcf-protection>
            # detect overflows of local arrays before the function returns
            $<$<BOOL:${<@ upper(namespace) @>_HAS_STACK_PROTECTOR}>:-fstack-protector-strong>
            # huge local variables can't reach past the end of the stack into other memory
            $<$<BOOL:${<@ upper(namespace) @>_HAS_CLASH_PROTECTION}>:-fstack-clash-protection>)

        # check memcpy/strcpy/sprintf and similar for buffer overflows
        target_compile_options(${TARGET_NAME} PRIVATE $<$<NOT:$<CONFIG:Debug>>:-U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=3>)

        target_link_options(${TARGET_NAME} PRIVATE
            # addresses of library functions become read-only after startup
            $<$<BOOL:${<@ upper(namespace) @>_HAS_FULL_RELRO}>:LINKER:-z,relro,-z,now>
            # data on the stack can never be run as code
            $<$<BOOL:${<@ upper(namespace) @>_HAS_NOEXECSTACK}>:LINKER:-z,noexecstack>)

        target_compile_definitions(${TARGET_NAME} PRIVATE
            _GLIBCXX_ASSERTIONS                                 # bounds checking for containers in libstdc++
            _LIBCPP_HARDENING_MODE=_LIBCPP_HARDENING_MODE_FAST) # bounds checking for containers in libc++
    endif()

endfunction()
