#include "options_validator.hpp"
#include "gtest/gtest.h"

using namespace gena;

namespace
{
    Options valid_options()
    {
        return Options{.name = "project",
                       .type = ProjectType::library,
                       .standard = CppStandard::cpp23,
                       .dependencies = Dependency::googletest,
                       .location = std::filesystem::current_path()};
    }
} // namespace

TEST(OptionsValidatorTest, ProjectName)
{
    Options options = valid_options();

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
    Options options = valid_options();
    options.type = static_cast<ProjectType>(0xCAFE);
    EXPECT_THROW(OptionsValidator::validate(options), std::invalid_argument) << "Type have to be a valid enum value";
}

TEST(OptionsValidatorTest, CppStandard)
{
    Options options = valid_options();
    options.standard = static_cast<CppStandard>(0xBEAF);
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

    Options options = valid_options();
    for (auto entry : dependencies)
    {
        options.dependencies = entry;
        EXPECT_THROW(OptionsValidator::validate(options), std::invalid_argument)
            << "Dependencies have to include exactly one test framework";
    }
}

TEST(OptionsValidatorTest, Location)
{
    Options options = valid_options();

    options.location = std::filesystem::path{};
    EXPECT_THROW(OptionsValidator::validate(options), std::invalid_argument) << "Location can't be empty";

    options.location = "non_exsitent";
    EXPECT_THROW(OptionsValidator::validate(options), std::invalid_argument) << "Location have to exist";

    options.location = "test_options_validator.exe";
    EXPECT_THROW(OptionsValidator::validate(options), std::invalid_argument) << "Location have to be the directory";
}
