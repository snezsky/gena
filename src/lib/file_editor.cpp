#include "file_editor.hpp"
#include "options_validator.hpp"

#include <inja/inja.hpp>

#include <unordered_set>

namespace gena
{
    struct FileEditor::Impl
    {
        explicit Impl(const RenderingOptions &options)
        {
            OptionsValidator::validate(options);

            /* Disable features we don't use to avoid unintended side effects */
            env_.set_html_autoescape(false);
            env_.set_comment("〈」〉", "〔「〕");
            env_.set_line_statement("「〉〔」〈〕");

            /* Set custom syntax to not mess with CMake */
            env_.set_statement("<%", "%>");
            env_.set_expression("<@", "@>");

            /* Hack to specify include directory for inja */
            env_.set_include_callback([this]([[maybe_unused]] const std::filesystem::path &, const std::string &name) {
                return env_.parse_file(includePath_ / name);
            });

            options_["project_name"] = options.name;
            options_["cpp_standard"] = options.standard;
            options_["project_type"] = project_type_to_string(options.type);
            options_["dependencies"] = compose_dependencies(options.dependencies);
        }

        void render_templates(const std::filesystem::path &file)
        {
            static const std::unordered_set<std::string> allowed{".cmake", ".hpp", ".cpp", ".json",
                                                                 ".txt",   ".md",  ".rc"};
            if (!allowed.contains(file.extension().string())) { return; }

            try
            {
                env_.render_file(file, options_);
            }
            catch (const inja::InjaError &e)
            {
                const std::string filename = std::filesystem::path{file}.make_preferred().string();
                throw std::runtime_error(
                    std::format("Error rendering template!\n\nFile: {}\nError: {}", filename, e.what()));
            }

            const std::string content = env_.render_file(file, options_);

            std::ofstream out;
            out.exceptions(std::ios::failbit | std::ios::badbit); // NOLINT(hicpp-signed-bitwise)
            out.open(file, std::ios::out | std::ios::trunc);      // NOLINT(hicpp-signed-bitwise)
            out << content;
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
        static std::string project_type_to_string(ProjectType type)
        {
            switch (type)
            {
            case ProjectType::library: return "library";
            case ProjectType::executable: return "executable";
            case ProjectType::qmainwindow: return "qmainwindow";
            }
            std::unreachable();
        }

        static std::vector<std::string> compose_dependencies(Dependencies deps)
        {
            std::vector<std::string> names;
            if (deps.testFlag(Dependency::json)) { names.emplace_back("json"); }
            if (deps.testFlag(Dependency::CLI11)) { names.emplace_back("CLI11"); }
            if (deps.testFlag(Dependency::spdlog)) { names.emplace_back("spdlog"); }
            if (deps.testFlag(Dependency::qtest)) { names.emplace_back("qtest"); }
            if (deps.testFlag(Dependency::catch2)) { names.emplace_back("catch2"); }
            if (deps.testFlag(Dependency::googletest)) { names.emplace_back("googletest"); }
            return names;
        }

      private:
        inja::Environment env_;
        nlohmann::json options_; // Directory where Inja looks for included template files
        std::filesystem::path includePath_ = std::filesystem::current_path() / "assets" / "include";
    };

    FileEditor::FileEditor(const RenderingOptions &options)
        : impl_{std::make_unique<Impl>(options)}
    {
    }

    FileEditor::~FileEditor() = default;

    void FileEditor::render_templates(const std::filesystem::path &file)
    {
        impl_->render_templates(file);
    }

    void FileEditor::replace_in_name(const std::filesystem::path &file, std::string_view before, std::string_view after)
    {
        FileEditor::Impl::replace_in_name(file, before, after);
    }
} // namespace gena
