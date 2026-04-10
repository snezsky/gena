#include "gtest/gtest.h"
#include "myproject.hpp"

TEST(StringManipulationTest, TrimmingChecks)
{
    EXPECT_EQ(myproject::trim(""), "");
    EXPECT_EQ(myproject::trim(" "), "");
    EXPECT_EQ(myproject::trim(" spaces "), "spaces");
    EXPECT_EQ(myproject::trim("nospace"), "nospace");
    EXPECT_EQ(myproject::trim(" leftspace"), "leftspace");
    EXPECT_EQ(myproject::trim("rightspace "), "rightspace");
    EXPECT_EQ(myproject::trim("    moreleftspaces "), "moreleftspaces");
    EXPECT_EQ(myproject::trim("morerightspaces    "), "morerightspaces");
}

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

TEST(StringManipulationTest, JoiningChecks)
{
    EXPECT_EQ(myproject::join({""}, ""), "");
    EXPECT_EQ(myproject::join({"two str", "ings"}, ""), "two strings");
    EXPECT_EQ(myproject::join({"two", "strings"}, " "), "two strings");
    EXPECT_EQ(myproject::join({"", "string", ""}, ""), "string");
    EXPECT_EQ(myproject::join({"", "string", ""}, "__"), "__string__");
}
