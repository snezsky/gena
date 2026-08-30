#pragma once

#include "generation_options.hpp"

namespace gena
{
    class IGitClient
    {
      public:
        virtual ~IGitClient() = default;

        virtual void set_repository_path(std::filesystem::path repository) = 0;

        virtual void init() = 0;
        virtual void add(const std::string &pathspec) = 0;
        virtual void commit(const std::string &message) = 0;
        virtual void add_submodule(const Submodule &submodule) = 0;
        virtual void add_submodules(const std::vector<Submodule> &submodules) = 0;
        virtual void set_execute_permission(const std::filesystem::path &file) = 0;
    };

    class GitClient : public IGitClient
    {
      public:
        GitClient() = default;
        explicit GitClient(std::filesystem::path repository);
        virtual ~GitClient() = default;

        void set_repository_path(std::filesystem::path repository) override;

        void init() override;

        void add(const std::string &pathspec) override;

        void commit(const std::string &message) override;

        void add_submodule(const Submodule &submodule) override;

        void add_submodules(const std::vector<Submodule> &submodules) override;

        void set_execute_permission(const std::filesystem::path &file) override;

      private:
        std::filesystem::path repository_;
    };
} // namespace gena
