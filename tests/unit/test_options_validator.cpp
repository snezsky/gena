#include "auxiliary.hpp"
#include "options_validator.hpp"
#include "gtest/gtest.h"

using namespace gena;

TEST(OptionsValidatorTest, ProjectName)
{
    GenerationOptions options = valid_options();

    options.name = "";
    EXPECT_THROW(OptionsValidator::validate(options), std::invalid_argument) << "Name can't be empty";

    options.name = "project-name";
    EXPECT_THROW(OptionsValidator::validate(options), std::invalid_argument) << "Name can't contain '-'";

    options.name = "7project";
    EXPECT_THROW(OptionsValidator::validate(options), std::invalid_argument) << "Name can't start with a digit";

    options.name = "проект";
    EXPECT_THROW(OptionsValidator::validate(options), std::invalid_argument) << "Name can't use non-english letters";
}

TEST(OptionsValidatorTest, ProjectType)
{
    GenerationOptions options = valid_options();
    options.type = static_cast<ProjectType>(0xCA);
    EXPECT_THROW(OptionsValidator::validate(options), std::invalid_argument) << "Type have to be a valid enum value";
}

TEST(OptionsValidatorTest, CppStandard)
{
    GenerationOptions options = valid_options();
    options.standard = static_cast<CppStandard>(0xFE);
    EXPECT_THROW(OptionsValidator::validate(options), std::invalid_argument)
        << "C++ standard have to be a valid enum value";
}

TEST(OptionsValidatorTest, Dependencies)
{
    static constexpr auto dependencies = {
        Dependencies{},
        Dependencies{Dependency::catch2, Dependency::qtest},
        Dependencies{Dependency::catch2, Dependency::googletest},
        Dependencies{Dependency::googletest, Dependency::qtest},
        Dependencies{Dependency::googletest, Dependency::catch2, Dependency::qtest},
    };

    GenerationOptions options = valid_options();
    for (auto entry : dependencies)
    {
        options.dependencies = entry;
        EXPECT_THROW(OptionsValidator::validate(options), std::invalid_argument)
            << "Dependencies have to include exactly one test framework";
    }
}

TEST(OptionsValidatorTest, Location)
{
    GenerationOptions options = valid_options();

    options.location = std::filesystem::path{};
    EXPECT_THROW(OptionsValidator::validate(options), std::invalid_argument) << "Location can't be empty";

    options.location = "non_exsitent";
    EXPECT_THROW(OptionsValidator::validate(options), std::invalid_argument) << "Location have to exist";

    options.location = "test_options_validator.exe";
    EXPECT_THROW(OptionsValidator::validate(options), std::invalid_argument) << "Location have to be the directory";
}
