#include <filesystem>
#include <string_view>

#include "options.hpp"

namespace gena
{
    class Generator
    {
      public:
        static void generate(const Options &options);

      private:
        using path = std::filesystem::path;

        static void copy_content(const path &source, const path &destination);
        static void copy_sources(const path &source, const path &destination, ProjectType projectType);
        static void copy_dependencies(const path &source, const path &destination, Dependencies dependencies);
        static void embed_project_name(const path &projectDir, std::string_view projectName);
        static void embed_cpp_standard(const path &projectDir, CppStandard standard);
        static void setup_git_repository(const path &projectDir);
    };
} // namespace gena
