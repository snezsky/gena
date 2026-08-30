#pragma once

#include <QUrl>
#include <QFlags>
#include <QObject>
#include <QVariantMap>
#include <QtQml/qqmlregistration.h>

#include <filesystem>

namespace gena
{
    Q_NAMESPACE
    QML_NAMED_ELEMENT(Options)

    enum class ProjectType : uint8_t
    {
        Library,
        ConsoleApplication,
        QtQuickApplication,
        QtWidgetsApplication,
    };
    Q_ENUM_NS(ProjectType)

    enum class CppStandard : uint8_t
    {
        Cpp17 = 17,
        Cpp20 = 20,
        Cpp23 = 23,
    };
    Q_ENUM_NS(CppStandard)

    enum class TestFramework : uint8_t
    {
        QTest = 1U << 1U,
        Catch2 = 1U << 2U,
        GoogleTest = 1U << 3U,
    };
    Q_ENUM_NS(TestFramework)

    struct GenerationOptions
    {
        std::string name;
        ProjectType type;
        CppStandard standard;
        TestFramework test_framework;
        std::string cpp_namespace;
        std::filesystem::path output_directory;
        std::vector<std::string> submodule_urls;
    };
} // namespace gena
