#pragma once

#include <algorithm>
#include <cctype>
#include <string>
#include <string_view>
#include <vector>

namespace gena
{
    inline std::string capitalize(const std::string &str)
    {
        if (str.empty()) { return ""; }

        std::string capitalized = str;
        capitalized[0] = static_cast<char>(std::toupper(static_cast<unsigned char>(capitalized[0])));
        return capitalized;
    }

    inline bool any_contains_case_insensitive(const std::vector<std::string> &haystack, std::string_view needle)
    {
        return std::ranges::any_of(haystack, [needle](const std::string &str) {
            return std::ranges::search(str, needle, [](char haystack_symbol, char needle_symbol) {
                       return std::tolower(static_cast<unsigned char>(haystack_symbol)) ==
                              std::tolower(static_cast<unsigned char>(needle_symbol));
                   }).begin() != str.end();
        });
    }
} // namespace gena
