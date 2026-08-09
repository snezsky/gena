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
    Q_FLAG_NS(Dependencies)
    Q_ENUM_NS(Dependency)

    struct GenerationOptions
    {
        std::string name;
        ProjectType type;
        CppStandard standard;
        Dependencies dependencies;
        std::string cpp_namespace;
        std::filesystem::path location;
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
                .dependencies = get_or_throw("dependencies").value<Dependencies>(),
                .cpp_namespace = get_or_throw("namespace").toString().toStdString(),
                .location = get_or_throw("location").toUrl().toLocalFile().toStdString(),
                .setup_git = get_or_throw("setupGit").toBool(),
            };
        }
    };
} // namespace gena
