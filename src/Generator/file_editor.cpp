#include "file_editor.hpp"
#include "options_validator.hpp"
#include "generation_options_to_strings.hpp"

#include <inja/inja.hpp>
#include <whereami/whereami.hpp>

#include <format>
#include <unordered_set>

namespace gena
{
    struct FileEditor::Impl
    {
        explicit Impl(const RenderingOptions &options)
        {
            OptionsValidator::validate(options);
            setup_inja_environment();
            setup_inja_data(options);
        }

        void render_templates(const std::filesystem::path &file)
        {
            static const std::unordered_set<std::string> allowed{".cmake", ".hpp", ".cpp", ".json",
                                                                 ".txt",   ".md",  ".rc"};
            if (!allowed.contains(file.extension().string())) { return; }

            try
            {
                const std::string content = env_.render_file(file, options_);

                std::ofstream out;
                out.exceptions(std::ios::failbit | std::ios::badbit); // NOLINT(hicpp-signed-bitwise)
                out.open(file, std::ios::out | std::ios::trunc);      // NOLINT(hicpp-signed-bitwise)
                out << content;
            }
            catch (const inja::InjaError &e)
            {
                const std::string filename = std::filesystem::path{file}.make_preferred().string();
                throw std::runtime_error(
                    std::format("Error rendering template!\n\nFile: {}\nError: {}", filename, e.what()));
            }
            catch (const std::system_error &e)
            {
                const std::string filename = std::filesystem::path{file}.make_preferred().string();
                throw std::runtime_error(
                    std::format("Error writing rendered template to file!\n\nFile: {}\nError: {}", filename, e.what()));
            }
        }

        static void replace_in_name(const std::filesystem::path &file, std::string_view before, std::string_view after)
        {
            std::string name = file.filename().string();
            if (name.contains(before))
            {
                name.replace(name.find(before), before.size(), after);
                std::filesystem::rename(file, file.parent_path() / name);
            }
        }

      private:
        void setup_inja_environment()
        {
            /* Disable features we don't use to avoid unintended side effects */
            env_.set_html_autoescape(false);
            env_.set_comment("〈」〉", "〔「〕");
            env_.set_line_statement("「〉〔」〈〕");

            /* Process whitespaces in a way that ignores separate lines with conditions/cycles */
            env_.set_trim_blocks(true);
            env_.set_lstrip_blocks(true);

            /* Set custom syntax to not mess with CMake */
            env_.set_statement("<%", "%>");
            env_.set_expression("<@", "@>");

            /* Hack to specify include directory for inja */
            env_.set_include_callback([this]([[maybe_unused]] const std::filesystem::path &, const std::string &name) {
                static const auto applicationPath = whereami::get_executable_directory();
                static const auto includePath = applicationPath / "assets" / "include";
                return env_.parse_file(includePath / name);
            });
        }

        void setup_inja_data(const RenderingOptions &options)
        {
            options_["project_name"] = options.name;
            options_["cpp_standard"] = options.standard;
            options_["project_type"] = to_string(options.type);
            options_["dependencies"] = to_strings(options.dependencies);
            options_["namespace"] = options.cpp_namespace;
        }

      private:
        inja::Environment env_;
        nlohmann::json options_; // Directory where Inja looks for included template files
    };

    FileEditor::FileEditor(const RenderingOptions &options)
        : impl_{std::make_unique<Impl>(options)}
    {
    }

    FileEditor::~FileEditor() = default;

    void FileEditor::render_templates(const std::filesystem::path &file)
    { impl_->render_templates(file); }

    void FileEditor::replace_in_name(const std::filesystem::path &file, std::string_view before, std::string_view after)
    { FileEditor::Impl::replace_in_name(file, before, after); }
} // namespace gena
