include("Utility.cmake")

add_executable(configured "empty.cpp")
<@ lower(namespace) @>_setup_target(configured)

assert_target_property_equal(configured CXX_EXTENSIONS OFF)
assert_target_property_equal(configured CXX_STANDARD_REQUIRED ON)
assert_target_property_equal(configured EXPORT_COMPILE_COMMANDS ON)

test_build_output(warnings_test "non_virtual_destructor.cpp" "virtual destructor|destructor is not virtual")
