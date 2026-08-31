#include "generation_options.hpp"
#include "rendering_options.hpp"

namespace gena
{
    class OptionsValidator
    {
      public:
        static void validate(const GenerationOptions &options);
        static void validate(const RenderingOptions &options);

      private:
        static void validate_name(const std::string &name);
        static void validate_type(ProjectType type);
        static void validate_cpp_standard(CppStandard standard);
        static void validate_test_framework(TestFramework testFramework);
        static void validate_namespace(const std::string &cpp_namespace);
        static void validate_submodules(const std::vector<std::string>& urls, TestFramework testFramework);
        static void validate_output_directory(const std::filesystem::path &outputDir, std::string_view projectName);
    };
} // namespace gena
