#pragma once

#include <filesystem>

namespace whereami
{
    /* Returns the path to the current executable */
    std::filesystem::path get_executable_path();

    /* Returns the directory containing the current executable. */
    std::filesystem::path get_executable_directory();

    /*  Returns the path to the current module */
    std::filesystem::path get_module_path();

    /* Returns the directory containing the current module. */
    std::filesystem::path get_module_directory();

} // namespace whereami
