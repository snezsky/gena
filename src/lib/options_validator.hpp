#include "options.hpp"

namespace gena
{
    class OptionsValidator
    {
      public:
        static void validate(const Options &options);

      private:
        static void validate_name(const std::string &name);
        static void validate_type(ProjectType type);
        static void validate_cpp_standard(CppStandard standard);
        static void validate_dependencies(Dependencies dependencies);
        static void validate_location(const std::filesystem::path &location);
    };
} // namespace gena
