function(__myproject_target_enable_cache TARGET_NAME)

  find_program(CACHE_BINARY NAMES "ccache")
  if (NOT CACHE_BINARY)
      find_program(CACHE_BINARY NAMES "sccache")
  endif()
  if(NOT CACHE_BINARY)
      message(AUTHOR_WARNING "Cache is enabled, but no cache binary was found. Please install ccache or sccache.")
      return()
  endif()

  set_target_properties(
      ${TARGET_NAME} PROPERTIES
      C_COMPILER_LAUNCHER   ${CACHE_BINARY}
      CXX_COMPILER_LAUNCHER ${CACHE_BINARY})

endfunction()
