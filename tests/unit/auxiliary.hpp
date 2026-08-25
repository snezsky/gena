#pragma once

#include "generation_options.hpp"
#include "generation_options_to_strings.hpp"

#include <QTemporaryDir>
#include <format>
#include <fstream>

namespace gena
{
    inline std::filesystem::path temp_directory()
    {
        static std::vector<QTemporaryDir> temps;

        QTemporaryDir temp;
        if (!temp.isValid())
        {
            const std::string error = temp.errorString().toStdString();
            throw std::runtime_error("Failed to create temporary directory: " + error);
        }

        temps.push_back(std::move(temp));
        return temps.back().path().toStdString();
    }

    inline GenerationOptions valid_options()
    {
        return GenerationOptions{.name = "project",
                                 .type = ProjectType::Library,
                                 .standard = CppStandard::Cpp23,
                                 .test_framework = TestFramework::QTest,
                                 .cpp_namespace = "prj",
                                 .output_directory = temp_directory(),
                                 .setup_git = false};
    }

    inline GenerationOptions options_with_invalid_standard()
    {
        gena::GenerationOptions options = valid_options();
        options.standard = static_cast<gena::CppStandard>(0xFE);
        return options;
    }

    inline std::filesystem::path would_be_project_directory(gena::GenerationOptions options)
    {
        /* Generator will not return project directory if
           generation failed, so we just return what it would be */
        return options.output_directory / options.name;
    }

    inline std::string content_of(std::filesystem::path file)
    {
        std::ifstream in(file);
        in.exceptions(std::ios::failbit | std::ios::badbit);
        return std::string(std::istreambuf_iterator<char>(in), {});
    }

    /* Prints options nicely in a test output */
    void PrintTo(const GenerationOptions &options, std::ostream *os)
    {
        *os << "options: ";
        *os << std::format("{}, ", options.name);
        *os << std::format("{}, ", options.cpp_namespace);
        *os << std::format("{}, ", to_string(options.type));
        *os << std::format("{}, ", to_string(options.standard));
        *os << std::format("{}, ", to_string(options.test_framework));
        *os << options.output_directory.string();
    }
} // namespace gena
