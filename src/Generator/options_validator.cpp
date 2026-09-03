#include "options_validator.hpp"
#include "string_extensions.hpp"

#include <QRegularExpression>

namespace gena
{
    void OptionsValidator::validate(const GenerationOptions &options)
    {
        validate(RenderingOptions{options});
        validate_submodules(options.submodule_urls, options.test_framework);
        validate_output_directory(options.output_directory, options.name);
    }

    void OptionsValidator::validate(const RenderingOptions &options)
    {
        validate_name(options.name);
        validate_type(options.type);
        validate_cpp_standard(options.standard);
        validate_test_framework(options.test_framework);
        validate_namespace(options.cpp_namespace);
    }

    void OptionsValidator::validate_name(const std::string &name)
    {
        static const QRegularExpression regexp{"^[A-Za-z][A-Za-z0-9]*(?:_[A-Za-z0-9]+)*$"};
        if (!regexp.match(QString::fromStdString(name)).hasMatch())
        {
            throw std::invalid_argument("Invalid project name! Use English letters, numbers and underscores only.");
        }
    }

    void OptionsValidator::validate_type(ProjectType type)
    {
        switch (type)
        {
        case ProjectType::Library:
        case ProjectType::ConsoleApplication:
        case ProjectType::QtQuickApplication:
        case ProjectType::QtWidgetsApplication: return;
        }
        throw std::invalid_argument("Invalid project type!");
    }

    void OptionsValidator::validate_cpp_standard(CppStandard standard)
    {
        switch (standard)
        {
        case CppStandard::Cpp17:
        case CppStandard::Cpp20:
        case CppStandard::Cpp23: return;
        }
        throw std::invalid_argument("Invalid C++ standard!");
    }

    void OptionsValidator::validate_test_framework(TestFramework testFramework)
    {
        switch (testFramework)
        {
        case TestFramework::QTest:
        case TestFramework::Catch2:
        case TestFramework::GoogleTest: return;
        }
        throw std::invalid_argument("Invalid test framework!");
    }

    void OptionsValidator::validate_namespace(const std::string &cpp_namespace)
    {
        static const QRegularExpression regexp{"^[A-Za-z][A-Za-z0-9]*(?:_[A-Za-z0-9]+)*$"};
        if (!regexp.match(QString::fromStdString(cpp_namespace)).hasMatch())
        {
            throw std::invalid_argument("Invalid C++ namespace! Use English letters, numbers and underscores only.");
        }
    }

    void OptionsValidator::validate_submodules(const std::vector<std::string> &urls, TestFramework testFramework)
    {
        static const QRegularExpression scpRegex(R"(^[^@\s]+@[^@:\s]+:[^\s]+$)");
        static const QRegularExpression urlRegex(R"(^(https?|ssh|git)://[^:/\s]+(?::\d+)?(?:/[^/\s]*)*$)");

        for (const auto &url : urls)
        {
            if (!urlRegex.match(QString::fromStdString(url)).hasMatch() &&
                !scpRegex.match(QString::fromStdString(url)).hasMatch())
            {
                throw std::invalid_argument("Invalid submodule url: " + url + "!\n" +
                                            "It should be a valid git repository url to clone.");
            }
        }

        if (testFramework == TestFramework::Catch2 && !any_contains_case_insensitive(urls, "/catch2"))
        {
            throw std::invalid_argument("You must include Catch2 as submodule to use it as test framework.");
        }

        if (testFramework == TestFramework::GoogleTest && !any_contains_case_insensitive(urls, "/googletest"))
        {
            throw std::invalid_argument("You must include googletest as submodule to use it as test framework.");
        }
    }

    void OptionsValidator::validate_output_directory(const std::filesystem::path &outputDir,
                                                     std::string_view projectName)
    {
        if (std::filesystem::path projectDir = outputDir / projectName;
            std::filesystem::is_directory(projectDir) && !std::filesystem::is_empty(projectDir))
        {
            throw std::invalid_argument("Directory '" + projectDir.make_preferred().string() + "' is not empty.");
        }

        if (!std::filesystem::exists(outputDir))
        {
            throw std::invalid_argument("Invalid output directory! Path does not exist.");
        }

        if (!std::filesystem::is_directory(outputDir))
        {
            throw std::invalid_argument("Invalid output directory! Path is not a directory.");
        }
    }
} // namespace gena
