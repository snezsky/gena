#include "qgenerator.hpp"
#include <thread>

namespace gena
{
    void QGenerator::generateAsync(const QVariantMap &options)
    {
        std::vector<Submodule> submodules;
        for (auto &submodule : options["submodules"].toList())
        {
            submodules.push_back(Submodule{.name = submodule.toMap()["name"].toString().toStdString(),
                                           .url = submodule.toMap()["url"].toString().toStdString()});
        }

        GenerationOptions cppOptions{
            .name = options["name"].toString().toStdString(),
            .type = options["type"].value<ProjectType>(),
            .standard = options["standard"].value<CppStandard>(),
            .test_framework = options["testFramework"].value<TestFramework>(),
            .cpp_namespace = options["namespace"].toString().toStdString(),
            .output_directory = options["outputDirectory"].toUrl().toLocalFile().toStdString(),
            .submodules = std::move(submodules),
            .setup_git = options["setupGit"].toBool(),
        };

        std::thread(&QGenerator::generate, this, cppOptions).detach();
    }

    void QGenerator::generate(const GenerationOptions &options)
    {
        try
        {
            generator_.generate(options);
            project_directory_ = QUrl::fromLocalFile(generator_.project_directory().string().c_str());
            emit generationFinished();
        }
        catch (const std::exception &e)
        {
            emit generationFailed(e.what());
            project_directory_.clear();
        }
        catch (...)
        {
            emit generationFailed("Something broke. It refused to say what");
            project_directory_.clear();
        }
    }
} // namespace gena
