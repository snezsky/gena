function(gena_enable_cache)

  if (DEFINED CMAKE_CXX_COMPILER_LAUNCHER)
    return()
  endif()

  find_program(CACHE_BINARY NAMES "ccache" "sccache")
  if (NOT CACHE_BINARY)
    message(STATUS "Neither ccache nor sccache found, building without a compiler cache")
    return()
  endif()

  set(CMAKE_C_COMPILER_LAUNCHER   ${CACHE_BINARY} PARENT_SCOPE)
  set(CMAKE_CXX_COMPILER_LAUNCHER ${CACHE_BINARY} PARENT_SCOPE)

endfunction()

gena_enable_cache()
