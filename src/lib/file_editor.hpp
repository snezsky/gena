#include "generation_options.hpp"

#include "inja/inja.hpp"

#include <filesystem>
#include <string_view>

namespace gena
{
    class FileEditor
    {
      public:
        /* Creates instance with `options` for template engine */
        explicit FileEditor(const GenerationOptions &options);

        /* Replaces all placeholders in `file` content with template engine */
        void render_templates(const std::filesystem::path &file);

        /* Renames `file`, replacing `before` to `after` */
        static void replace_in_name(const std::filesystem::path &file, std::string_view before, std::string_view after);

      private:
        static std::string project_type_to_string(ProjectType type);
        static std::vector<std::string> compose_dependencies(Dependencies deps);

      private:
        inja::Environment env_;
        nlohmann::json options_;
    };

} // namespace gena
