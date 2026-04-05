#include <fstream>
#include <iostream>

#include "file_editor.hpp"
#include "generator.hpp"

using namespace gena;

namespace
{
    Options parse_options([[maybe_unused]] std::span<char *> args)
    {
        if (args.size() != 2)
        {
            throw std::invalid_argument("expected exactly one argument: the project name [type][cpp_standard]");
        }

        const QString projectName{args[1]};

        Options options;
        options.name = projectName.toStdString();
        options.location = std::filesystem::current_path();
        options.dependencies = Dependency::catch2;

        if (projectName.startsWith("lib")) { options.type = ProjectType::lib; }
        else if (projectName.startsWith("exe")) { options.type = ProjectType::exe; }
        else { throw std::invalid_argument("unknown project type"); }

        if (projectName.endsWith("17")) { options.standard = CppStandard::cpp17; }
        else if (projectName.endsWith("20")) { options.standard = CppStandard::cpp20; }
        else if (projectName.endsWith("23")) { options.standard = CppStandard::cpp23; }
        else { throw std::invalid_argument("unknown std standard"); }

        return options;
    }

    /* The idea is to run generator tests from the generated project with ctest */
    void inject_generator_tests(const QString &projectName, const std::filesystem::path &projectTestDir)
    {
        static const std::string folderToInject = "injected";

        std::filesystem::copy(std::filesystem::current_path() / folderToInject, projectTestDir / folderToInject,
                              std::filesystem::copy_options::recursive);

        QFile utilities(projectTestDir / folderToInject / "Utility.cmake");
        QFile cmakelist(projectTestDir / folderToInject / "CMakeLists.txt");
        FileEditor::replace_in_content(utilities, "myproject", projectName);
        FileEditor::replace_in_content(utilities, "MYPROJECT", projectName.toUpper());
        FileEditor::replace_in_content(cmakelist, "myproject", projectName);

        std::ofstream out(projectTestDir / "CMakeLists.txt", std::ios::app);
        out << "add_subdirectory(" + folderToInject + ")\n";
    }
} // namespace

// NOLINTNEXTLINE(bugprone-exception-escape)
int main(int argc, char *argv[])
{
    try
    {
        const std::span<char *> args(argv, static_cast<size_t>(argc));
        const Options options = parse_options(args);
        const std::filesystem::path projectTestDir = options.location / options.name / "tests";

        Generator::generate(options);
        inject_generator_tests(QString::fromStdString(options.name), projectTestDir);
    }
    catch (const std::exception &e)
    {
        std::cout << "Generation failed with exception:\n" << e.what() << '\n';
    }
}
