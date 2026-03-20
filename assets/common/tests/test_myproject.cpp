#include "catch2/catch_test_macros.hpp"
#include "myproject.hpp"

TEST_CASE("Trimming checks")
{
    CHECK(myproject::trim("") == "");
    CHECK(myproject::trim(" ") == "");
    CHECK(myproject::trim("  ") == "");
    CHECK(myproject::trim(" spaces ") == "spaces");
    CHECK(myproject::trim("nospace") == "nospace");
    CHECK(myproject::trim(" leftspace") == "leftspace");
    CHECK(myproject::trim("rightspace ") == "rightspace");
    CHECK(myproject::trim("  morespaces    ") == "morespaces");
    CHECK(myproject::trim("    moreleftspaces ") == "moreleftspaces");
    CHECK(myproject::trim("morerightspaces    ") == "morerightspaces");
}

TEST_CASE("Splitting checks")
{
    using strings = std::vector<std::string>;

    CHECK(myproject::split("", ' ') == strings{});
    CHECK(myproject::split("", 'x') == strings{});
    CHECK(myproject::split("with_delimiter", '_') == strings{"with", "delimiter"});
    CHECK(myproject::split("withoutYdelimiter", 'N') == strings{"withoutYdelimiter"});
    CHECK(myproject::split(" beginning with", ' ') == strings{"", "beginning", "with"});
    CHECK(myproject::split("ending_with_", '_') == strings{"ending", "with"});
    CHECK(myproject::split("=sequ==ential==", '=') == strings{"", "sequ", "", "ential", ""});
    CHECK(myproject::split("a lot of delimiters", ' ') == strings{"a", "lot", "of", "delimiters"});
}

TEST_CASE("Joining checks")
{
    CHECK(myproject::join({""}, "") == "");
    CHECK(myproject::join({"two str", "ings"}, "") == "two strings");
    CHECK(myproject::join({"two", "strings"}, " ") == "two strings");
    CHECK(myproject::join({"", "string", ""}, "") == "string");
    CHECK(myproject::join({"", "string", ""}, "__") == "__string__");
}
