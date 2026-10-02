#pragma once

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <string>

namespace gena
{
    inline std::string to_lowercase(const std::string &str)
    {
        std::string lowercased = str;
        std::ranges::transform(lowercased, lowercased.begin(), [](unsigned char symbol) {
            return static_cast<char>(std::tolower(symbol));
        });
        return lowercased;
    }

    inline std::string capitalize(const std::string &str)
    {
        if (str.empty()) { return ""; }

        std::string capitalized = str;
        capitalized[0] = static_cast<char>(std::toupper(static_cast<unsigned char>(capitalized[0])));
        return capitalized;
    }

    /* Unlike path::string(), doesn't throw on Windows when the path has symbols outside the ANSI code page */
    inline std::string to_utf8(const std::filesystem::path &path)
    {
        const std::u8string utf8 = path.u8string();
        return {utf8.begin(), utf8.end()};
    }
} // namespace gena
