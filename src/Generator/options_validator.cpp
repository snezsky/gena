#include "options_validator.hpp"
#include "git_client.hpp"
#include "string_extensions.hpp"

#include <QRegularExpression>

#include <format>
#include <unordered_map>

namespace gena
{
    void OptionsValidator::validate(const GenerationOptions &options)
    {
        validate(RenderingOptions{options});
        validate_submodule_urls(options.submodule_urls, options.test_framework);
        validate_submodule_names(options.submodule_urls);
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
        // clang-format off
        static const QRegularExpression regexp{QRegularExpression::anchoredPattern("[A-Za-z][A-Za-z0-9]*(?:_[A-Za-z0-9]+)*")};
        constexpr auto cmakeKeywords = std::to_array<std::string_view>({"all", "clean", "help", "install", "test"});
        // clang-format on

        if (!regexp.match(QString::fromStdString(name)).hasMatch())
        {
            throw std::invalid_argument("Invalid project name! Use English letters, numbers and underscores only.");
        }

        if (std::ranges::contains(cmakeKeywords, name))
        {
            throw std::invalid_argument("Invalid project name! '" + name + "' is a reserved target name.");
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

    void OptionsValidator::validate_namespace(const std::string &cppNamespace)
    {
        // clang-format off
        static const QRegularExpression regex{ QRegularExpression::anchoredPattern("[A-Za-z][A-Za-z0-9]*(?:_[A-Za-z0-9]+)*") };
        constexpr auto cppKeywords = std::to_array<std::string_view>({ "alignas", "alignof", "and", "and_eq", "asm", "atomic_cancel", "atomic_commit", "atomic_noexcept", "auto", "bitand", "bitor", "bool", "break", "case", "catch", "char", "char8_t", "char16_t", "char32_t", "class", "compl", "concept", "const", "consteval", "constexpr", "constinit", "const_cast", "continue", "contract_assert", "co_await", "co_return", "co_yield", "decltype", "default", "delete", "do", "double", "dynamic_cast", "else", "enum", "explicit", "export", "extern", "false", "float", "for", "friend", "goto", "if", "inline", "int", "long", "mutable", "namespace", "new", "noexcept", "not", "not_eq", "nullptr", "operator", "or", "or_eq", "private", "protected", "public", "reflexpr", "register", "reinterpret_cast", "requires", "return", "short", "signed", "sizeof", "static", "static_assert", "static_cast", "struct", "switch", "synchronized", "template", "this", "thread_local", "throw", "true", "try", "typedef", "typeid", "typename", "union", "unsigned", "using", "virtual", "void", "volatile", "wchar_t", "while", "xor", "xor_eq" });
        // clang-format on

        if (!regex.match(QString::fromStdString(cppNamespace)).hasMatch())
        {
            throw std::invalid_argument("Invalid C++ namespace! Use English letters, numbers and underscores only.");
        }

        if (std::ranges::contains(cppKeywords, cppNamespace))
        {
            throw std::invalid_argument("Invalid C++ namespace! '" + cppNamespace + "' is a reserved keyword.");
        }
    }

    void OptionsValidator::validate_submodule_urls(const std::vector<std::string> &urls, TestFramework testFramework)
    {
        static const QRegularExpression scpRegex(
            QRegularExpression::anchoredPattern(R"([^@\s]+@[^@:\s]+:[^\s]+)"));
        static const QRegularExpression urlRegex(
            QRegularExpression::anchoredPattern(R"((https?|ssh|git)://[^:/\s]+(?::\d+)?(?:/[^/\s]*)*)"));

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

    void OptionsValidator::validate_submodule_names(const std::vector<std::string> &urls)
    {
        std::unordered_map<std::string, std::string> urlByName;
        for (const auto &url : urls)
        {
            const std::string name = GitClient::repository_name(url);
            if (name.empty())
            {
                throw std::invalid_argument("Cannot determine repository name from url: " + url + "!");
            }

            const auto [existing, inserted] = urlByName.try_emplace(to_lowercase(name), url);
            if (!inserted)
            {
                throw std::invalid_argument(std::format(
                    "Submodules must have unique names, but {} and {} are both named {}", url, existing->second, name));
            }
        }
    }

    void OptionsValidator::validate_output_directory(const std::filesystem::path &outputDir,
                                                     std::string_view projectName)
    {
        if (!std::filesystem::exists(outputDir))
        {
            throw std::invalid_argument("Invalid output directory! Path does not exist.");
        }

        if (!std::filesystem::is_directory(outputDir))
        {
            throw std::invalid_argument("Invalid output directory! Path is not a directory.");
        }

        const auto projectDir = std::filesystem::path{outputDir / projectName}.make_preferred();

        if (std::filesystem::is_directory(projectDir) && !std::filesystem::is_empty(projectDir))
        {
            throw std::invalid_argument("Directory '" + projectDir.string() + "' is not empty.");
        }

        if (std::filesystem::exists(projectDir) && !std::filesystem::is_directory(projectDir))
        {
            throw std::invalid_argument("Path '" + projectDir.string() + "' is not a directory.");
        }
    }
} // namespace gena
