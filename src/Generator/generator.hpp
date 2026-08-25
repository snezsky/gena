#pragma once

#include <filesystem>
#include <string_view>

#include "generation_options.hpp"

namespace gena
{
    class Generator
    {
      public:
        void generate(const GenerationOptions &options);

        std::filesystem::path project_directory() const;

      private:
        using path = std::filesystem::path;

        static void copy_sources(const path &source, const path &destination, ProjectType projectType);
        static void copy_tests(const path &source, const path &destination, TestFramework testFramework);
        static void copy_test_framework(const path &source, const path &destination, TestFramework testFramework);
        static void render_templates(const path &projectDir, const GenerationOptions &options);
        static void setup_git_repository(const path &projectDir);

        static void copy_content(const path &source, const path &destination);

      private:
        path projectDirectory_;
    };
} // namespace gena
