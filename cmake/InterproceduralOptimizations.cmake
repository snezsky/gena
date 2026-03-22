include(CheckIPOSupported)

function (__gena_target_enable_interprocedural_optimizations TARGET_NAME)

    check_ipo_supported(RESULT IPO_SUPPORTED)
    if (IPO_SUPPORTED)
        set_target_properties(
            ${TARGET_NAME} PROPERTIES
            INTERPROCEDURAL_OPTIMIZATION ON)
    endif()

endfunction()
