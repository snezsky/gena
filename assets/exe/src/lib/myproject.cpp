#include "myproject.hpp"

#include <sstream>

namespace myproject
{
    std::string trim(const std::string &str)
    {
        auto isSpace = [](unsigned char symbol) {
            return std::isspace(symbol) != 0;
        };

        size_t start = 0;
        while (start < str.size() && isSpace(str[start]))
        {
            ++start;
        }

        size_t end = str.size();
        while (end > start && isSpace(str[end - 1]))
        {
            --end;
        }

        return str.substr(start, end - start);
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
