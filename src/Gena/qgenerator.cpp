#include "qgenerator.hpp"
#include <thread>

namespace gena
{
    void QGenerator::generateAsync(const QVariantMap &options)
    {
        std::vector<std::string> submoduleUrls;
        for (auto &submodule : options["submodules"].toStringList())
        {
            submoduleUrls.push_back(submodule.toStdString());
        }

        GenerationOptions cppOptions{
            .name = options["name"].toString().toStdString(),
            .type = options["type"].value<ProjectType>(),
            .standard = options["standard"].value<CppStandard>(),
            .test_framework = options["testFramework"].value<TestFramework>(),
            .cpp_namespace = options["namespace"].toString().toStdString(),
            .output_directory = options["outputDirectory"].toUrl().toLocalFile().toStdString(),
            .submodule_urls = std::move(submoduleUrls),
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
