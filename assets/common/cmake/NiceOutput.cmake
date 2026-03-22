function(__myproject_target_enable_nice_output TARGET_NAME)

target_compile_options(
    ${TARGET_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/diagnostics:column>
    $<$<CXX_COMPILER_ID:GNU>:-fdiagnostics-color=always>
    $<$<CXX_COMPILER_ID:Clang>:-fcolor-diagnostics>
)

endfunction() 
