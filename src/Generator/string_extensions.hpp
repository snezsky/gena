#pragma once

#include <algorithm>
#include <cctype>
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
} // namespace gena
