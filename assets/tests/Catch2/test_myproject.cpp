#include "catch2/catch_test_macros.hpp"
#include "<@ project_name @>/<@ project_name @>.hpp"

TEST_CASE("Splitting checks")
{
    using strings = std::vector<std::string>;

    CHECK(<@ project_name @>::split("", ' ') == strings{});
    CHECK(<@ project_name @>::split("", 'x') == strings{});
    CHECK(<@ project_name @>::split("with_delimiter", '_') == strings{"with", "delimiter"});
    CHECK(<@ project_name @>::split("withoutYdelimiter", 'N') == strings{"withoutYdelimiter"});
    CHECK(<@ project_name @>::split(" beginning with", ' ') == strings{"", "beginning", "with"});
    CHECK(<@ project_name @>::split("ending_with_", '_') == strings{"ending", "with"});
    CHECK(<@ project_name @>::split("=sequ==ential==", '=') == strings{"", "sequ", "", "ential", ""});
    CHECK(<@ project_name @>::split("a lot of delimiters", ' ') == strings{"a", "lot", "of", "delimiters"});
}
