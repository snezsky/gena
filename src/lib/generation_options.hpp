#pragma once

#include <QFlags>
#include <filesystem>

namespace gena
{
    enum class ProjectType : uint8_t
    {
        Library,
        Executable,
        QMainWindow,
    };

    enum class CppStandard : uint8_t
    {
        Cpp17 = 17,
        Cpp20 = 20,
        Cpp23 = 23,
    };

    enum class Dependency : uint8_t
    {
        Json = 1U << 0U,
        CLI11 = 1U << 1U,
        Spdlog = 1U << 2U,
        QTest = 1U << 3U,
        Catch2 = 1U << 4U,
        GoogleTest = 1U << 5U,
    };
    Q_DECLARE_FLAGS(Dependencies, Dependency);

    struct GenerationOptions
    {
        std::string name;
        ProjectType type;
        CppStandard standard;
        Dependencies dependencies;
        std::string cpp_namespace;
        std::filesystem::path location;
        bool setup_git = false;
    };
} // namespace gena
