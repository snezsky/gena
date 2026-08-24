function(gena_prevent_in_source)

  file(REAL_PATH "${CMAKE_SOURCE_DIR}" SRC_DIR)
  file(REAL_PATH "${CMAKE_BINARY_DIR}" BIN_DIR)

  if ("${SRC_DIR}" STREQUAL "${BIN_DIR}")
    message(FATAL_ERROR [[
     In-source builds are disabled.
     Please, run CMake from a separate build directory.
    ]])
  endif()
  
endfunction()

gena_prevent_in_source()
