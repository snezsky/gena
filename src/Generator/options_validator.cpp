#include "options_validator.hpp"

#include <QRegularExpression>

namespace gena
{
    void OptionsValidator::validate(const GenerationOptions &options)
    {
        validate(RenderingOptions{options});
        validate_location(options.location, options.name);
    }

    void OptionsValidator::validate(const RenderingOptions &options)
    {
        validate_name(options.name);
        validate_type(options.type);
        validate_cpp_standard(options.standard);
        validate_dependencies(options.dependencies);
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

    void OptionsValidator::validate_dependencies(Dependencies dependencies)
    {
        const int testFrameworksCount = static_cast<int>(dependencies.testFlag(Dependency::QTest)) +
                                        static_cast<int>(dependencies.testFlag(Dependency::Catch2)) +
                                        static_cast<int>(dependencies.testFlag(Dependency::GoogleTest));
        if (testFrameworksCount != 1)
        {
            throw std::invalid_argument("Invalid dependencies! You must choose exactly one test framework.");
        }
    }

    void OptionsValidator::validate_namespace(const std::string &cpp_namespace)
    {
        static const QRegularExpression regexp{"^[A-Za-z][A-Za-z0-9]*(?:_[A-Za-z0-9]+)*$"};
        if (!regexp.match(QString::fromStdString(cpp_namespace)).hasMatch())
        {
            throw std::invalid_argument("Invalid C++ namespace! Use English letters, numbers and underscores only.");
        }
    }

    void OptionsValidator::validate_location(const std::filesystem::path &location, std::string_view projectName)
    {
        if (std::filesystem::path projectDir = location / projectName;
            std::filesystem::is_directory(projectDir) && !std::filesystem::is_empty(projectDir))
        {
            throw std::invalid_argument("Directory '" + projectDir.make_preferred().string() + "' is not empty.");
        }

        if (!std::filesystem::exists(location))
        {
            throw std::invalid_argument("Invalid project location! Path does not exist.");
        }

        if (!std::filesystem::is_directory(location))
        {
            throw std::invalid_argument("Invalid project location! Path is not a directory.");
        }
    }
} // namespace gena
