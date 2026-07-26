#include "generator.hpp"
#include "file_editor.hpp"
#include "options_validator.hpp"
#include "whereami/whereami.hpp"

#include <fstream>

#include <QProcess>

namespace fs = std::filesystem;

namespace gena
{
    void Generator::generate(const GenerationOptions &options)
    {
        try
        {
            OptionsValidator::validate(options);
        }
        catch (...)
        {
            projectDirectory_.clear();
            throw;
        }

        const fs::path source = whereami::get_executable_directory() / "assets";
        const fs::path destination = options.location / options.name;

        try
        {
            copy_content(source / "common", destination);
            copy_sources(source, destination, options.type);
            copy_tests(source / "tests", destination / "tests", options.dependencies);

            render_templates(destination, options);

            copy_dependencies(source / "deps", destination / "deps", options.dependencies);
            if (options.setup_git) { setup_git_repository(destination); }

            projectDirectory_ = destination;
        }
        catch (...)
        {
            projectDirectory_.clear();
            fs::remove_all(destination);
            throw;
        }
    }

    std::filesystem::path Generator::project_directory() const
    {
        if (projectDirectory_.empty())
        {
            throw std::runtime_error("Cannot retrieve project directory before a successful generation");
        }
        return projectDirectory_;
    }

    void Generator::copy_sources(const path &source, const path &destination, ProjectType projectType)
    {
        switch (projectType)
        {
        case ProjectType::Library: copy_content(source / "type" / "library", destination); break;
        case ProjectType::Executable: copy_content(source / "type" / "executable", destination); break;
        case ProjectType::QMainWindow: copy_content(source / "type" / "qmainwindow", destination); break;
        }
    }

    void Generator::copy_tests(const path &source, const path &destination, Dependencies dependencies)
    {
        if (dependencies.testFlag(Dependency::QTest)) { copy_content(source / "QtTest", destination); }
        if (dependencies.testFlag(Dependency::Catch2)) { copy_content(source / "Catch2", destination); }
        if (dependencies.testFlag(Dependency::GoogleTest)) { copy_content(source / "googletest", destination); }
    }

    void Generator::copy_dependencies(const path &source, const path &destination, Dependencies dependencies)
    {
        /* The qtest dependency does not require any files to be copied */
        if (!dependencies || dependencies == Dependency::QTest) { return; }

        auto add_dependency = [&source, &destination](std::string_view name) {
            fs::copy(source / name, destination / name, fs::copy_options::recursive);
            std::ofstream out(destination / "CMakeLists.txt", std::ios::app);
            out << "add_subdirectory(\"" << name << "\")\n";
        };

        fs::create_directories(destination);
        fs::copy_file(source / "CMakeLists.txt", destination / "CMakeLists.txt");

        if (dependencies.testFlag(Dependency::Json)) { add_dependency("json"); }
        if (dependencies.testFlag(Dependency::CLI11)) { add_dependency("CLI11"); }
        if (dependencies.testFlag(Dependency::Spdlog)) { add_dependency("spdlog"); }
        if (dependencies.testFlag(Dependency::Catch2)) { add_dependency("Catch2"); }
        if (dependencies.testFlag(Dependency::GoogleTest)) { add_dependency("googletest"); }
    }

    void Generator::render_templates(const path &projectDir, const GenerationOptions &options)
    {
        /* Process deepest paths first. Renaming a parent invalidates children */
        std::vector<fs::path> entries(fs::recursive_directory_iterator(projectDir), {});
        std::ranges::sort(entries, [](const fs::path &lhs, const fs::path &rhs) {
            return std::distance(lhs.begin(), lhs.end()) > std::distance(rhs.begin(), rhs.end());
        });

        FileEditor editor{options};
        for (const fs::path &entry : entries)
        {
            editor.render_templates(entry);
            FileEditor::replace_in_name(entry, "myproject", options.name);
        }
    }

    void Generator::setup_git_repository(const std::filesystem::path &projectDir)
    {
        auto git = [&](const QStringList &args) {
            QProcess process;
            process.setWorkingDirectory(QString::fromStdString(projectDir.string()));
            process.start("git", args);

            if (!process.waitForFinished() || process.exitStatus() != QProcess::NormalExit ||
                process.exitCode() != EXIT_SUCCESS)
            {
                const std::string command = args.join(' ').toStdString();
                const std::string error = process.readAllStandardError().toStdString();
                throw std::runtime_error(
                    std::format("Failed to setup git repository!\nCommand: {}\nError: {}", command, error));
            }
        };

        git({"init"});
        git({"add", "."});
        git({"update-index", "--chmod=+x", "scripts/coverage.sh"});
        git({"add", "."});
        git({"commit", "-m", "create initial project structure"});
    }

    void Generator::copy_content(const fs::path &source, const fs::path &destination)
    {
        fs::create_directories(destination);

        for (const auto &entry : fs::directory_iterator(source))
        {
            fs::copy(entry, destination / entry.path().filename(), fs::copy_options::recursive);
        }
    }
} // namespace gena
