#include "qgenerator.hpp"
#include <thread>

namespace gena
{
    void QGenerator::generateAsync(const QVariantMap &options)
    {
        const GenerationOptions cpp_options = GenerationOptions::fromQVariantMap(options);
        std::thread(&QGenerator::generate, this, cpp_options).detach();
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
