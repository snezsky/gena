#include "generator.hpp"
#include "file_editor.hpp"
#include "options.hpp"
#include "options_validator.hpp"

#include <fstream>

#include <QCoreApplication>
#include <QProcess>

namespace fs = std::filesystem;

namespace gena
{
    void Generator::generate(const Options &options)
    {
        OptionsValidator::validate(options);

        const fs::path pwd{QCoreApplication::applicationDirPath().toStdU16String()};
        const fs::path source = pwd / "assets";
        const fs::path destination = options.location / options.name;

        copy_content(source / "common", destination);
        copy_sources(source, destination, options.type);
        copy_tests(source / "tests", destination / "tests", options.dependencies);

        render_templates(destination, options);

        copy_dependencies(source / "deps", destination / "deps", options.dependencies);
        setup_git_repository(destination);
    }

    void Generator::copy_sources(const path &source, const path &destination, ProjectType projectType)
    {
        switch (projectType)
        {
        case ProjectType::library: copy_content(source / "type" / "library", destination); break;
        case ProjectType::executable: copy_content(source / "type" / "executable", destination); break;
        case ProjectType::qmainwindow: copy_content(source / "type" / "qmainwindow", destination); break;
        }
    }

    void Generator::copy_tests(const path &source, const path &destination, Dependencies dependencies)
    {
        if (dependencies.testFlag(Dependency::qtest)) { copy_content(source / "QtTest", destination); }
        if (dependencies.testFlag(Dependency::catch2)) { copy_content(source / "Catch2", destination); }
        if (dependencies.testFlag(Dependency::googletest)) { copy_content(source / "googletest", destination); }
    }

    void Generator::copy_dependencies(const path &source, const path &destination, Dependencies dependencies)
    {
        if (!dependencies) { return; }

        auto add_dependency = [&source, &destination](std::string_view name) {
            fs::copy(source / name, destination / name, fs::copy_options::recursive);
            std::ofstream out(destination / "CMakeLists.txt", std::ios::app);
            out << "add_subdirectory(\"" << name << "\")\n";
        };

        fs::create_directories(destination);
        fs::copy_file(source / "CMakeLists.txt", destination / "CMakeLists.txt");

        if (dependencies.testFlag(Dependency::json)) { add_dependency("json"); }
        if (dependencies.testFlag(Dependency::CLI11)) { add_dependency("CLI11"); }
        if (dependencies.testFlag(Dependency::spdlog)) { add_dependency("spdlog"); }
        if (dependencies.testFlag(Dependency::catch2)) { add_dependency("Catch2"); }
        if (dependencies.testFlag(Dependency::googletest)) { add_dependency("googletest"); }
    }

    void Generator::render_templates(const path &projectDir, const Options &options)
    {
        FileEditor editor{options};

        auto dirIt = fs::recursive_directory_iterator(projectDir);
        for (const auto &entry : dirIt)
        {
            editor.render_templates(entry.path());
            FileEditor::replace_in_name(entry.path(), "myproject", options.name);
        }
    }

    void Generator::setup_git_repository(const std::filesystem::path &projectDir)
    {
        if (QProcess::execute("git", {"--version"}) != 0) { return; }

        auto git = [&](const QStringList &args) {
            QProcess process;
            process.setWorkingDirectory(QString::fromStdString(projectDir.string()));
            process.start("git", args);

            if (!process.waitForFinished() || process.exitStatus() != QProcess::NormalExit ||
                process.exitCode() != EXIT_SUCCESS)
            {
                const std::string command = args.join(' ').toStdString();
                throw std::runtime_error("Failed to execute git command: " + command);
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
