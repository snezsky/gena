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
    constexpr std::string_view to_string(TestFramework testFramework)
    {
        switch (testFramework)
        {
        case TestFramework::QTest: return "QTest";
        case TestFramework::Catch2: return "Catch2";
        case TestFramework::GoogleTest: return "googletest";
        }
        std::unreachable();
    }
} // namespace gena
