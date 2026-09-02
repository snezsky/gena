#pragma once

#include <filesystem>
#include <string_view>

#include "generation_options.hpp"
#include "git_client.hpp"

namespace gena
{
    class Generator
    {
      public:
        void generate(const GenerationOptions &options,
                      std::unique_ptr<IGitClient> gitClient = std::make_unique<GitClient>());

        std::filesystem::path project_directory() const;

      private:
        using path = std::filesystem::path;

        static void copy_sources(const path &source, const path &destination, ProjectType projectType);
        static void copy_tests(const path &source, const path &destination, TestFramework testFramework);
        static void create_submodules_directory(const path &destination, const std::vector<std::string> &submoduleUrls);
        static void render_templates(const path &projectDir, const GenerationOptions &options);
        static void setup_git_repository(const path &projectDir, const std::vector<std::string> &submoduleUrls,
                                         std::unique_ptr<IGitClient> gitClient);
        static void copy_content(const path &source, const path &destination);

      private:
        path projectDirectory_;
    };
} // namespace gena
