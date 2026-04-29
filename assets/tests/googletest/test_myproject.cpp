#include "gtest/gtest.h"
#include "<@ project_name @>/<@ project_name @>.hpp"

TEST(StringManipulationTest, SplittingChecks)
{
    using strings = std::vector<std::string>;

    EXPECT_EQ(<@ project_name @>::split("", ' '), strings{});
    EXPECT_EQ(<@ project_name @>::split("", 'x'), strings{});
    EXPECT_EQ(<@ project_name @>::split("with_delimiter", '_'), strings({"with", "delimiter"}));
    EXPECT_EQ(<@ project_name @>::split("withoutYdelimiter", 'N'), strings({"withoutYdelimiter"}));
    EXPECT_EQ(<@ project_name @>::split(" beginning with", ' '), strings({"", "beginning", "with"}));
    EXPECT_EQ(<@ project_name @>::split("ending_with_", '_'), strings({"ending", "with"}));
    EXPECT_EQ(<@ project_name @>::split("=sequ==ential==", '='), strings({"", "sequ", "", "ential", ""}));
    EXPECT_EQ(<@ project_name @>::split("a lot of delimiters", ' '), strings({"a", "lot", "of", "delimiters"}));
}
