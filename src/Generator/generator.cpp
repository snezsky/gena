#include "generator.hpp"
#include "file_editor.hpp"
#include "generation_options_to_strings.hpp"
#include "options_validator.hpp"

#include "whereami/whereami.hpp"

#include <fstream>

#include <QProcess>

namespace fs = std::filesystem;

namespace
{
    std::string capitalize(const std::string &str)
    {
        if (str.empty()) { return ""; }

        std::string capitalized = str;
        capitalized[0] = static_cast<char>(std::toupper(static_cast<unsigned char>(capitalized[0])));
        return capitalized;
    }
} // namespace

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
        const fs::path destination = options.output_directory / options.name;

        try
        {
            copy_content(source / "common", destination);
            copy_sources(source, destination, options.type);
            copy_tests(source / "tests", destination / "tests", options.test_framework);

            render_templates(destination, options);

            copy_test_framework(source / "test_frameworks", destination / "deps", options.test_framework);

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
    { copy_content(source / "type" / to_string(projectType), destination); }

    void Generator::copy_tests(const path &source, const path &destination, TestFramework testFramework)
    { copy_content(source / to_string(testFramework), destination); }

    void Generator::copy_test_framework(const path &source, const path &destination, TestFramework testFramework)
    {
        /* The qtest does not require any files to be copied */
        if (testFramework == TestFramework::QTest) { return; }

        const std::string_view testFrameworkName = to_string(testFramework);

        fs::create_directories(destination);
        fs::copy_file(source / "CMakeLists.txt", destination / "CMakeLists.txt");
        fs::copy(source / testFrameworkName, destination / testFrameworkName, fs::copy_options::recursive);

        std::ofstream out(destination / "CMakeLists.txt", std::ios::app);
        out << "add_subdirectory(\"" << testFrameworkName << "\")\n";
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
            FileEditor::replace_in_name(entry, "Myproject", capitalize(options.name));
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
