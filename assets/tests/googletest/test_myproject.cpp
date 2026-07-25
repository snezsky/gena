#include "gtest/gtest.h"
#include "<@ project_name @>/<@ project_name @>.hpp"

TEST(StringManipulationTest, SplittingChecks)
{
    using strings = std::vector<std::string>;

    EXPECT_EQ(<@ namespace @>::split("", ' '), strings{});
    EXPECT_EQ(<@ namespace @>::split("", 'x'), strings{});
    EXPECT_EQ(<@ namespace @>::split("with_delimiter", '_'), strings({"with", "delimiter"}));
    EXPECT_EQ(<@ namespace @>::split("withoutYdelimiter", 'N'), strings({"withoutYdelimiter"}));
    EXPECT_EQ(<@ namespace @>::split(" beginning with", ' '), strings({"", "beginning", "with"}));
    EXPECT_EQ(<@ namespace @>::split("ending_with_", '_'), strings({"ending", "with"}));
    EXPECT_EQ(<@ namespace @>::split("=sequ==ential==", '='), strings({"", "sequ", "", "ential", ""}));
    EXPECT_EQ(<@ namespace @>::split("a lot of delimiters", ' '), strings({"a", "lot", "of", "delimiters"}));
}
