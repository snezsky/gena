#include "gtest/gtest.h"
#include "myproject.hpp"

TEST(StringManipulationTest, SplittingChecks)
{
    using strings = std::vector<std::string>;

    EXPECT_EQ(myproject::split("", ' '), strings{});
    EXPECT_EQ(myproject::split("", 'x'), strings{});
    EXPECT_EQ(myproject::split("with_delimiter", '_'), strings({"with", "delimiter"}));
    EXPECT_EQ(myproject::split("withoutYdelimiter", 'N'), strings({"withoutYdelimiter"}));
    EXPECT_EQ(myproject::split(" beginning with", ' '), strings({"", "beginning", "with"}));
    EXPECT_EQ(myproject::split("ending_with_", '_'), strings({"ending", "with"}));
    EXPECT_EQ(myproject::split("=sequ==ential==", '='), strings({"", "sequ", "", "ential", ""}));
    EXPECT_EQ(myproject::split("a lot of delimiters", ' '), strings({"a", "lot", "of", "delimiters"}));
}
