#include "catch2/catch_test_macros.hpp"
#include "<@ project_name @>/<@ project_name @>.hpp"

TEST_CASE("Splitting checks")
{
    using strings = std::vector<std::string>;

    CHECK(<@ namespace @>::split("", ' ') == strings{});
    CHECK(<@ namespace @>::split("", 'x') == strings{});
    CHECK(<@ namespace @>::split("with_delimiter", '_') == strings{"with", "delimiter"});
    CHECK(<@ namespace @>::split("withoutYdelimiter", 'N') == strings{"withoutYdelimiter"});
    CHECK(<@ namespace @>::split(" beginning with", ' ') == strings{"", "beginning", "with"});
    CHECK(<@ namespace @>::split("ending_with_", '_') == strings{"ending", "with"});
    CHECK(<@ namespace @>::split("=sequ==ential==", '=') == strings{"", "sequ", "", "ential", ""});
    CHECK(<@ namespace @>::split("a lot of delimiters", ' ') == strings{"a", "lot", "of", "delimiters"});
}
