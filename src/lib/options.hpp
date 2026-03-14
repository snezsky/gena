#pragma once

#include <QIcon>
#include <QString>

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
        none = 0,
        json = 1 << 0,
        CL11 = 1 << 1,
        spdlog = 1 << 2,
        catch2 = 1 << 4,
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
