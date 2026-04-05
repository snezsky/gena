include("Utility.cmake")

add_executable(configured "empty.cpp")
myproject_setup_target(configured)

assert_target_property_equal(configured CXX_EXTENSIONS OFF)
assert_target_property_equal(configured CXX_STANDARD_REQUIRED ON)
assert_target_property_equal(configured EXPORT_COMPILE_COMMANDS ON)
assert_target_property_equal(configured INTERPROCEDURAL_OPTIMIZATION ON)

test_build_output(clang_tidy_test "magic_number.cpp" "readability-magic-numbers")
test_build_output(warnings_test "non_virtual_destructor.cpp" "virtual destructor")
test_build_output(cppcheck_test "unused_private_function.cpp" "Unused private function")

test_program_output(address_sanitizer_test "buffer_overflow.cpp" "ERROR: AddressSanitizer")
