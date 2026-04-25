#include <filesystem>
#include <string_view>

#include "generation_options.hpp"

namespace gena
{
    class Generator
    {
      public:
        static void generate(const GenerationOptions &options);

      private:
        using path = std::filesystem::path;

        static void copy_sources(const path &source, const path &destination, ProjectType projectType);
        static void copy_tests(const path &source, const path &destination, Dependencies dependencies);
        static void copy_dependencies(const path &source, const path &destination, Dependencies dependencies);
        static void render_templates(const path &projectDir, const GenerationOptions& options);
        static void setup_git_repository(const path &projectDir);

        static void copy_content(const path &source, const path &destination);
    };
} // namespace gena
