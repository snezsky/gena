#include "whereami/whereami.h"
#include "whereami/whereami.hpp"

#include <functional>
#include <string>

namespace whereami
{
    std::filesystem::path get_executable_path()
    {
        const int length = wai_getExecutablePath(nullptr, 0, nullptr);
        if (length != -1)
        {
            std::string path(length, '\0');
            wai_getExecutablePath(path.data(), path.size(), nullptr);
            return path;
        }

        throw std::runtime_error("Failed to retrieve the executable path");
    }

    std::filesystem::path get_executable_directory()
    { return get_executable_path().parent_path(); }

    std::filesystem::path get_module_path()
    {
        const int length = wai_getModulePath(nullptr, 0, nullptr);
        if (length != -1)
        {
            std::string path(length, '\0');
            wai_getModulePath(path.data(), path.size(), nullptr);
            return path;
        }

        throw std::runtime_error("Failed to retrieve the module path");
    }

    std::filesystem::path get_module_directory()
    { return get_module_path().parent_path(); }

} // namespace whereami
