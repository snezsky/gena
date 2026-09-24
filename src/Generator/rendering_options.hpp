#pragma once

#include "generation_options.hpp"

namespace gena
{
    struct RenderingOptions
    {
        std::string name;
        ProjectType type;
        CppStandard standard;
        TestFramework test_framework;
        std::string cpp_namespace;
        std::vector<std::string> submodule_urls;

        // cppcheck-suppress noExplicitConstructor
        RenderingOptions(const GenerationOptions &options)
            : name{options.name},
              type{options.type},
              standard{options.standard},
              test_framework{options.test_framework},
              cpp_namespace{options.cpp_namespace},
              submodule_urls{options.submodule_urls}
        {
        }
    };
} // namespace gena
