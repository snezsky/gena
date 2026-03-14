#pragma once

#include <string>
#include <vector>

namespace myproject
{
    /*! Remove whitespace from both ends */
    [[nodiscard]]
    std::string trim(const std::string &str);

    /*! Split string by delimiter */
    [[nodiscard]]
    std::vector<std::string> split(const std::string &str, char delimiter);

    /*! Join strings with delimiter */
    [[nodiscard]]
    std::string join(const std::vector<std::string> &parts, const std::string &delimiter);
} // namespace myproject
