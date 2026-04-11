#include "options_validator.hpp"

#include <QRegularExpression>

void gena::OptionsValidator::validate(const Options &options)
{
    validate_name(options.name);
    validate_type(options.type);
    validate_cpp_standard(options.standard);
    validate_dependencies(options.dependencies);
    validate_location(options.location);
}

void gena::OptionsValidator::validate_name(const std::string &name)
{
    static const QRegularExpression regexp{"^[A-Za-z][A-Za-z0-9]*(?:_[A-Za-z0-9]+)*$"};
    if (!regexp.match(QString::fromStdString(name)).hasMatch())
    {
        throw std::invalid_argument("Invalid project name! Use English letters, numbers and underscores only.");
    }
}

void gena::OptionsValidator::validate_type(ProjectType type)
{
    switch (type)
    {
    case ProjectType::exe: [[fallthrough]];
    case ProjectType::lib: break;
    default: throw std::invalid_argument("Invalid project type!");
    }
}

void gena::OptionsValidator::validate_cpp_standard(CppStandard standard)
{
    switch (standard)
    {
    case CppStandard::cpp17: [[fallthrough]];
    case CppStandard::cpp20: [[fallthrough]];
    case CppStandard::cpp23: break;
    default: throw std::invalid_argument("Invalid C++ standard!");
    }
}

void gena::OptionsValidator::validate_dependencies(Dependencies dependencies)
{
    const int testFrameworksCount = static_cast<int>(dependencies.testFlag(Dependency::qtest)) +
                                    static_cast<int>(dependencies.testFlag(Dependency::catch2)) +
                                    static_cast<int>(dependencies.testFlag(Dependency::googletest));
    if (testFrameworksCount != 1)
    {
        throw std::invalid_argument("Invalid dependencies! You must choose exactly one test framework.");
    }
}

void gena::OptionsValidator::validate_location(const std::filesystem::path &location)
{
    if (!std::filesystem::exists(location))
    {
        throw std::invalid_argument("Invalid project location! Path does not exist.");
    }

    if (!std::filesystem::is_directory(location))
    {
        throw std::invalid_argument("Invalid project location! Path is not a directory.");
    }
}
