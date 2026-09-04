#include <fstream>
#include <iostream>
#include <span>

#include <QString>

#include "file_editor.hpp"
#include "generator.hpp"

using namespace gena;

namespace
{
    GenerationOptions parse_options([[maybe_unused]] std::span<char *> args)
    {
        if (args.size() != 2)
        {
            throw std::invalid_argument("expected exactly one argument: the project name [type][cpp_standard]");
        }

        const QString projectName{args[1]};

        GenerationOptions options;
        options.name = projectName.toStdString();
        options.output_directory = std::filesystem::current_path();
        options.cpp_namespace = options.name;

        if (projectName.startsWith("library")) { options.type = ProjectType::Library; }
        else if (projectName.startsWith("quick")) { options.type = ProjectType::QtQuickApplication; }
        else if (projectName.startsWith("console")) { options.type = ProjectType::ConsoleApplication; }
        else if (projectName.startsWith("widgets")) { options.type = ProjectType::QtWidgetsApplication; }
        else { throw std::invalid_argument("unknown project type"); }

        if (projectName.endsWith("17")) { options.standard = CppStandard::Cpp17; }
        else if (projectName.endsWith("20")) { options.standard = CppStandard::Cpp20; }
        else if (projectName.endsWith("23")) { options.standard = CppStandard::Cpp23; }
        else { throw std::invalid_argument("unknown c++ standard"); }

        /* Map each C++ standard to a test framework for full coverage */
        if (projectName.endsWith("17")) { options.test_framework = TestFramework::QTest; }
        else if (projectName.endsWith("20"))
        {
            options.test_framework = TestFramework::Catch2;
            options.submodule_urls = {"https://github.com/catchorg/Catch2.git"};
        }
        else if (projectName.endsWith("23"))
        {
            options.test_framework = TestFramework::GoogleTest;
            options.submodule_urls = {"https://github.com/google/googletest.git"};
        }

        return options;
    }

    /* The idea is to run generator tests from the generated project with ctest */
    void inject_generator_tests(const GenerationOptions &options, const std::filesystem::path &projectTestDir)
    {
        static const std::string folderToInject = "injected";

        std::filesystem::copy(std::filesystem::current_path() / folderToInject, projectTestDir / folderToInject,
                              std::filesystem::copy_options::recursive);

        FileEditor editor{options};
        editor.render_templates(projectTestDir / folderToInject / "Utility.cmake");
        editor.render_templates(projectTestDir / folderToInject / "CMakeLists.txt");

        std::ofstream out(projectTestDir / "CMakeLists.txt", std::ios::app);
        out << "add_subdirectory(" + folderToInject + ")\n";
    }
} // namespace

// NOLINTNEXTLINE(bugprone-exception-escape)
int main(int argc, char **argv)
{
    try
    {
        const std::span<char *> args(argv, static_cast<size_t>(argc));
        const GenerationOptions options = parse_options(args);

        Generator generator;
        generator.generate(options);
        inject_generator_tests(options, generator.project_directory() / "tests");
    }
    catch (const std::exception &e)
    {
        std::cout << "Generation failed with exception: " << e.what() << '\n';
        return EXIT_FAILURE;
    }
}
