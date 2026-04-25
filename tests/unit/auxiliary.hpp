#pragma once

#include "generation_options.hpp"
#include <fstream>

namespace gena
{
    inline GenerationOptions valid_options()
    {
        return GenerationOptions{.name = "project",
                                 .type = ProjectType::library,
                                 .standard = CppStandard::cpp23,
                                 .dependencies = Dependency::googletest,
                                 .location = std::filesystem::current_path()};
    }

    inline std::string content_of(std::filesystem::path file)
    {
        std::ifstream in(file, std::ios::binary);
        in.exceptions(std::ios::failbit | std::ios::badbit);
        return std::string(std::istreambuf_iterator<char>(in), {});
    }

    /* Prints options nicely in a test output */
    void PrintTo(const GenerationOptions &options, std::ostream *os)
    {
        *os << std::format("options ( {}", options.name);
        *os << std::format(", type({})", std::to_underlying(options.type));
        *os << std::format(", cpp{}", std::to_underlying(options.standard));
        *os << std::format(", deps({})", options.dependencies.toInt());
        *os << std::format(", {} )", options.location.string());
    }
} // namespace gena
