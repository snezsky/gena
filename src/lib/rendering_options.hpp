#pragma once

#include "generation_options.hpp"

namespace gena
{
    struct RenderingOptions
    {
        std::string name;
        ProjectType type;
        CppStandard standard;
        Dependencies dependencies;

        // cppcheck-suppress noExplicitConstructor
        RenderingOptions(const GenerationOptions &options)
            : name{options.name},
              type{options.type},
              standard{options.standard},
              dependencies{options.dependencies}
        {
        }
    };
} // namespace gena
