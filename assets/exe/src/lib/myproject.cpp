#include "myproject.hpp"

#include <sstream>

namespace myproject
{
    std::string trim(const std::string &str)
    {
        auto not_space = [](unsigned char symbol) {
            return std::isspace(symbol) == 0;
        };

        auto begin = std::find_if(str.begin(), str.end(), not_space);
        auto end = std::find_if(str.rbegin(), str.rend(), not_space).base();

        return begin >= end ? "" : std::string(begin, end);
    }

    std::vector<std::string> split(const std::string &str, char delimiter)
    {
        std::vector<std::string> result;
        std::stringstream sstream(str);
        std::string item;

        while (std::getline(sstream, item, delimiter))
        {
            result.push_back(item);
        }

        return result;
    }

    std::string join(const std::vector<std::string> &parts, const std::string &delimiter)
    {
        if (parts.empty()) { return ""; }

        std::string result = parts[0];
        for (size_t i = 1; i < parts.size(); ++i)
        {
            result += delimiter;
            result += parts[i];
        }

        return result;
    }
} // namespace myproject
