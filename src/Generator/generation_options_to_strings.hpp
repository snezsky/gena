#pragma once

#include <string_view>
#include <vector>

#include "generation_options.hpp"

namespace gena
{
    [[nodiscard]]
    constexpr std::string_view to_string(ProjectType type)
    {
        switch (type)
        {
        case ProjectType::Library: return "Library";
        case ProjectType::ConsoleApplication: return "ConsoleApplication";
        case ProjectType::QtQuickApplication: return "QtQuickApplication";
        case ProjectType::QtWidgetsApplication: return "QtWidgetsApplication";
        }
        std::unreachable();
    }

    [[nodiscard]]
    constexpr std::string_view to_string(CppStandard standard)
    {
        switch (standard)
        {
        case CppStandard::Cpp17: return "C++17";
        case CppStandard::Cpp20: return "C++20";
        case CppStandard::Cpp23: return "C++23";
        }
        std::unreachable();
    }

    [[nodiscard]]
    constexpr std::string_view to_string(Dependency dependency)
    {
        switch (dependency)
        {
        case Dependency::Json: return "json";
        case Dependency::CLI11: return "CLI11";
        case Dependency::Spdlog: return "spdlog";
        case Dependency::QTest: return "QTest";
        case Dependency::Catch2: return "Catch2";
        case Dependency::GoogleTest: return "googletest";
        }
        std::unreachable();
    }

    [[nodiscard]]
    constexpr std::vector<std::string_view> to_strings(Dependencies dependencies)
    {
        static constexpr auto allDeps = {Dependency::Json,  Dependency::CLI11,  Dependency::Spdlog,
                                         Dependency::QTest, Dependency::Catch2, Dependency::GoogleTest};

        std::vector<std::string_view> result;
        for (const auto &dependency : allDeps)
        {
            if (dependencies.testFlag(dependency)) { result.push_back(to_string(dependency)); }
        }
        return result;
    }
} // namespace gena
