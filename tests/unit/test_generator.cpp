#include <gtest/gtest.h>

#include "auxiliary.hpp"
#include "generator.hpp"


TEST(TestGenerator, DoesNotCreateDirectoryIfOptionsInvalid)
{
    gena::Generator generator;
    gena::GenerationOptions options = gena::valid_options();
    options.standard = static_cast<gena::CppStandard>(0xFF); // invalidates

    EXPECT_THROW(generator.generate(options), std::invalid_argument);
    EXPECT_FALSE(std::filesystem::exists(generator.project_directory()));
}

TEST(TestGenerator, DoesNotModifyExistingDirectoryIfOptionsInvalid)
{
    gena::Generator generator;
    gena::GenerationOptions options = gena::valid_options();
    options.name = "do_not_modify";
    options.location = std::filesystem::current_path() / "assets";
    options.standard = static_cast<gena::CppStandard>(0xFF); // invalidates options

    EXPECT_THROW(generator.generate(options), std::invalid_argument);
    EXPECT_TRUE(std::filesystem::exists(generator.project_directory()));
    EXPECT_TRUE(std::filesystem::exists(generator.project_directory() / "exist.txt"));
}
