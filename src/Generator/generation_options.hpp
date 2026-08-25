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
        bool setup_git = false;

        static GenerationOptions fromQVariantMap(const QVariantMap &map)
        {
            auto get_or_throw = [&map](const QString &key) {
                if (!map.contains(key))
                {
                    throw std::runtime_error("Generation options missing required key: " + key.toStdString());
                }
                return map[key];
            };

            return GenerationOptions{
                .name = get_or_throw("name").toString().toStdString(),
                .type = get_or_throw("type").value<ProjectType>(),
                .standard = get_or_throw("standard").value<CppStandard>(),
                .test_framework = get_or_throw("testFramework").value<TestFramework>(),
                .cpp_namespace = get_or_throw("namespace").toString().toStdString(),
                .output_directory = get_or_throw("outputDirectory").toUrl().toLocalFile().toStdString(),
                .setup_git = get_or_throw("setupGit").toBool(),
            };
        }
    };
} // namespace gena
