#include "generator.hpp"
#include "file_editor.hpp"
#include "options.hpp"

#include <fstream>

namespace fs = std::filesystem;

namespace gena
{
    void Generator::generate(const Options &options)
    {
        const fs::path source = fs::current_path() / "assets";
        const fs::path destination = options.location / options.name;

        copy_content(source / "common", destination);
        switch (options.type)
        {
        case ProjectType::exe: copy_content(source / "exe", destination); break;
        case ProjectType::lib: copy_content(source / "lib", destination); break;
        }
        embed_project_name(destination, options.name);
        embed_cpp_standard(destination, options.standard);
        copy_dependencies(source / "deps", destination / "deps", options.dependencies);
        setup_git_repository(destination);
    }

    void Generator::copy_content(const fs::path &source, const fs::path &destination)
    {
        fs::create_directories(destination);

        for (const auto &entry : fs::directory_iterator(source))
        {
            fs::copy(entry, destination / entry.path().filename(), fs::copy_options::recursive);
        }
    }

    void Generator::copy_dependencies(const path &source, const path &destination, Dependencies dependencies)
    {
        if (dependencies == Dependency::none) { return; }

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
    }

    void Generator::embed_project_name(const path &projectDir, std::string_view projectName)
    {
        const QString name = QString::fromStdString(std::string{projectName});
        const QString NAME = name.toUpper();

        auto dirIt = fs::recursive_directory_iterator(projectDir);
        for (const auto &entry : dirIt)
        {
            QFile file(QString::fromStdString(entry.path().string()));
            FileEditor::replace_in_name(file, "myproject", name);
            FileEditor::replace_in_content(file, "myproject", name);
            FileEditor::replace_in_content(file, "MYPROJECT", NAME);
        }
    }

    void Generator::embed_cpp_standard(const path &projectDir, CppStandard standard)
    {
        QFile file(projectDir / "cmake" / "Setup.cmake");
        const auto cppVer = static_cast<std::underlying_type_t<CppStandard>>(standard);

        FileEditor::replace_in_content(file, "20", QString::number(cppVer));
    }

    void Generator::setup_git_repository(const path &projectDir)
    {
        if (std::system("git --version") != 0) { return; }

        auto git = [&](const std::string &cmd) {
            const std::string full = "git -C \"" + projectDir.string() + "\" " + cmd;
            const int result = std::system(full.c_str());
            if (result != 0) { throw std::runtime_error("Failed to execute git command \"" + cmd + "\""); }
        };

        git("init");
        git("add .");
        git("update-index --chmod=+x scripts/coverage.sh");
        git("commit -m \"create initial project structure\"");
    }
} // namespace gena
