function(__gena_target_enable_static_analysis TARGET_NAME)

    if (NOT GENA_ENABLE_STATIC_ANALYSIS)
        return()
    endif()
    
    __gena_target_enable_cppcheck(${TARGET_NAME})
    __gena_target_enable_clang_tidy(${TARGET_NAME})

endfunction()


function(__gena_target_enable_cppcheck TARGET_NAME)

    find_program(CPPCHECK cppcheck)
    if (NOT CPPCHECK)
        message(FATAL_ERROR "static analysis enabled, but cppcheck was not found")
    endif()

    get_target_property(CPP_STANDARD ${TARGET_NAME} CXX_STANDARD)
    set(SUPPRESS_DIR "*:${CMAKE_CURRENT_BINARY_DIR}/_deps/*.h")
    if(MSVC)
        set(CPPCHECK_TEMPLATE vs)
    else()
        set(CPPCHECK_TEMPLATE gcc)
    endif()

    set(CPPCHECK_COMMAND ${CPPCHECK}
        --std=c++${CPP_STANDARD}
        --suppress=${SUPPRESS_DIR}
        --template=${CPPCHECK_TEMPLATE}
        --enable=style,performance,warning,portability
        --inline-suppr
        --suppress=unknownMacro
        --suppress=cppcheckError
        --suppress=internalAstError
        --suppress=unmatchedSuppression
        --suppress=passedByValue
        --suppress=syntaxError
        --suppress=preprocessorErrorDirective
        --suppress=knownConditionTrueFalse
        --inconclusive
        --error-exitcode=2)

    set_target_properties(
        ${TARGET_NAME} PROPERTIES
        CXX_CPPCHECK "${CPPCHECK_COMMAND}")

endfunction()


function(__gena_target_enable_clang_tidy TARGET_NAME)

    find_program(CLANG_TIDY clang-tidy)
    if(NOT CLANG_TIDY)
        message(FATAL_ERROR "static analysis enabled, but clang-tidy was not found")
    endif()

    get_target_property(CPP_STANDARD ${TARGET_NAME} CXX_STANDARD)
    if(MSVC)
        # clang does not support /Md + /fsanitize=address on Windows,
        # but we do not need clang-tidy to run clang with sanitizer
        set(IGNORE_SANITIZER "--removed-arg=/fsanitize=address")
        set(CPP_STANDARD_ARG "/std:c++${CPP_STANDARD}")
    else()
        set(CPP_STANDARD_ARG "-std=c++${CPP_STANDARD}")
    endif()

    set(CLANG_TIDY_COMMAND ${CLANG_TIDY}
        ${IGNORE_SANITIZER}
        -extra-arg=${CPP_STANDARD_ARG}
        -extra-arg=-Wno-unknown-warning-option
        -extra-arg=-Wno-ignored-optimization-argument
        -extra-arg=-Wno-unused-command-line-argument
        -warnings-as-errors=*
        -p)

    set_target_properties(
        ${TARGET_NAME} PROPERTIES
        CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}")

endfunction()
