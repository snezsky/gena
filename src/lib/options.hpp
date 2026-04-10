#pragma once

#include <QFlags>
#include <filesystem>

namespace gena
{
    enum class ProjectType : uint8_t
    {
        lib,
        exe,
    };

    enum class CppStandard : uint8_t
    {
        cpp17 = 17,
        cpp20 = 20,
        cpp23 = 23,
    };

    enum class Dependency : uint8_t
    {
        json = 1 << 0,
        CLI11 = 1 << 1,
        spdlog = 1 << 2,
        qtest = 1 << 3,
        catch2 = 1 << 4,
        googletest = 1 << 5,
    };
    Q_DECLARE_FLAGS(Dependencies, Dependency);

    struct Options
    {
        std::string name;
        ProjectType type;
        CppStandard standard;
        Dependencies dependencies;
        std::filesystem::path location;
    };
} // namespace gena
