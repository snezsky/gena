#include "git_client.hpp"

#include <QProcess>
#include <QStringList>

namespace
{
    void git(const std::filesystem::path &repository, const QStringList &args)
    {
        if (!std::filesystem::is_directory(repository))
        {
            throw std::invalid_argument(std::format("Invalid git repository path: {}", repository.string()));
        }

        QProcess process;
        process.setWorkingDirectory(QString::fromStdString(repository.string()));
        process.start("git", QStringList(args.begin(), args.end()));

        if (!process.waitForFinished() || process.exitStatus() != QProcess::NormalExit ||
            process.exitCode() != EXIT_SUCCESS)
        {
            const std::string command = args.join(' ').toStdString();
            const std::string error = process.readAllStandardError().toStdString();
            throw std::runtime_error(
                std::format("Error occurred during git operation!\nCommand: {}\nError: {}", command, error));
        }
    }
} // namespace

namespace gena
{
    GitClient::GitClient(std::filesystem::path repository)
        : repository_{std::move(repository)}
    {
    }

    void GitClient::set_repository_path(std::filesystem::path repository)
    { repository_ = std::move(repository); }

    void GitClient::init()
    { git(repository_, {"init"}); }

    void GitClient::add(const std::string &pathspec)
    { git(repository_, {"add", QString::fromStdString(pathspec)}); }

    void GitClient::commit(const std::string &message)
    { git(repository_, {"commit", "-m", QString::fromStdString(message)}); }

    void GitClient::add_submodule(const Submodule &submodule)
    {
        const QString url = QString::fromStdString(submodule.url);
        const QString name = QString::fromStdString("deps/" + submodule.name);

        git(repository_, {"submodule", "add", url, name});
        git(repository_, {"submodule", "update", "--init", "--recursive"});
    }

    void GitClient::add_submodules(const std::vector<Submodule> &submodules)
    {
        for (const auto &submodule : submodules)
        {
            add_submodule(submodule);
        }
    }

    void GitClient::set_execute_permission(const std::filesystem::path &file)
    { git(repository_, {"update-index", "--chmod=+x", QString::fromStdString(file.string())}); }
} // namespace gena
