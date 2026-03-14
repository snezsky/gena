#include "generator.hpp"
#include "options.hpp"
#include "replacements.hpp"

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
        if (dependencies.testFlag(Dependency::CL11)) { add_dependency("CL11"); }
        if (dependencies.testFlag(Dependency::spdlog)) { add_dependency("spdlog"); }
        if (dependencies.testFlag(Dependency::catch2)) { add_dependency("catch2"); }
    }

    void Generator::embed_project_name(const path &projectDir, std::string_view projectName)
    {
        const QString name = QString::fromStdString(std::string{projectName});
        const QString NAME = name.toUpper();

        auto dirIt = fs::recursive_directory_iterator(projectDir);
        for (const auto &entry : dirIt)
        {
            QFile file(QString::fromStdString(entry.path().string()));
            Replacements::replace_in_name(file, "myproject", name);
            Replacements::replace_in_content(file, "myproject", name);
            Replacements::replace_in_content(file, "MYPROJECT", NAME);
        }
    }

    void Generator::embed_cpp_standard(const path &projectDir, CppStandard standard)
    {
        const auto cppVer = static_cast<std::underlying_type_t<CppStandard>>(standard);
        const QString before = R"((CXX_STANDARD\s+)(\d+))";
        const QString after = QString("\\1%1").arg(cppVer);
        QFile file(projectDir / "cmake" / "setup.cmake");

        Replacements::replace_in_content(file, before, after);
    }
} // namespace gena
