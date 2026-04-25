#include "rendering_options.hpp"

#include <filesystem>
#include <string_view>

namespace gena
{
    class FileEditor
    {
      public:
        /* Creates instance with `options` for template engine */
        explicit FileEditor(const RenderingOptions &options);

        /* Have to declare because of pimpl */
        ~FileEditor();

        /* Replaces all placeholders in `file` content with template engine */
        void render_templates(const std::filesystem::path &file);

        /* Renames `file`, replacing `before` to `after` */
        static void replace_in_name(const std::filesystem::path &file, std::string_view before, std::string_view after);

      private:
        struct Impl;
        std::unique_ptr<Impl> impl_;
    };

} // namespace gena
