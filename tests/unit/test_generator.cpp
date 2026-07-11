#include <gtest/gtest.h>

#include "auxiliary.hpp"
#include "generator.hpp"


TEST(TestGenerator, DoesNotCreateDirectoryIfOptionsInvalid)
{
    gena::GenerationOptions options = gena::valid_options();
    options.standard = static_cast<gena::CppStandard>(0xFF); // invalidates

    EXPECT_THROW(gena::Generator::generate(options), std::invalid_argument);
    EXPECT_FALSE(std::filesystem::exists(options.location / options.name));
}

TEST(TestGenerator, DoesNotModifyExistingDirectoryIfOptionsInvalid)
{
    gena::GenerationOptions options = gena::valid_options();
    options.name = "do_not_modify";
    options.location = std::filesystem::current_path() / "assets";
    options.standard = static_cast<gena::CppStandard>(0xFF); // invalidates options

    /* Project directory will be `options.location / options.name` */

    EXPECT_THROW(gena::Generator::generate(options), std::invalid_argument);
    EXPECT_TRUE(std::filesystem::exists(options.location / options.name));
    EXPECT_TRUE(std::filesystem::exists(options.location / options.name / "exist.txt"));
}
