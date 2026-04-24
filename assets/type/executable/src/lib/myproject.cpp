#include "<@ project_name @>.hpp"

#include <sstream>

namespace <@project_name @>
{
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
} // namespace <@ project_name @>
