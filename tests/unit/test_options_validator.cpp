#include "auxiliary.hpp"
#include "options_validator.hpp"
#include "gtest/gtest.h"

using namespace gena;

template <typename T> struct OptionsValidatorTest : public testing::Test
{
    T options = valid_options();
};
    
using OptionsTypes = testing::Types<GenerationOptions, RenderingOptions>;
TYPED_TEST_SUITE(OptionsValidatorTest, OptionsTypes);

TYPED_TEST(OptionsValidatorTest, ProjectName)
{
    this->options.name = "";
    EXPECT_THROW(OptionsValidator::validate(this->options), std::invalid_argument) << "Empty names must not be allowed";

    this->options.name = " ";
    EXPECT_THROW(OptionsValidator::validate(this->options), std::invalid_argument)
        << "Names consisting of whitespaces must not be allowed";

    this->options.name = " ";
    EXPECT_THROW(OptionsValidator::validate(this->options), std::invalid_argument)
        << "Names consisting whitespaces in the middle must not be allowed";

    this->options.name = "project-name";
    EXPECT_THROW(OptionsValidator::validate(this->options), std::invalid_argument)
        << "Names containing '-' must not be allowed";

    this->options.name = "7project";
    EXPECT_THROW(OptionsValidator::validate(this->options), std::invalid_argument)
        << "Names starting with a digit must not be allowed";

    this->options.name = "проект";
    EXPECT_THROW(OptionsValidator::validate(this->options), std::invalid_argument)
        << "Names containing non-ascii symbols must not be allowed";
}

TYPED_TEST(OptionsValidatorTest, CppNamespace)
{
    this->options.cpp_namespace = "";
    EXPECT_THROW(OptionsValidator::validate(this->options), std::invalid_argument) << "Empty namespaces must not be allowed";

    this->options.cpp_namespace = " ";
    EXPECT_THROW(OptionsValidator::validate(this->options), std::invalid_argument)
        << "Namespaces consisting of whitespaces must not be allowed";

    this->options.cpp_namespace = " ";
    EXPECT_THROW(OptionsValidator::validate(this->options), std::invalid_argument)
        << "Namespaces consisting whitespaces in the middle must not be allowed";

    this->options.cpp_namespace = "namespace-name";
    EXPECT_THROW(OptionsValidator::validate(this->options), std::invalid_argument)
        << "Namespaces containing '-' must not be allowed";

    this->options.cpp_namespace = "7namespace";
    EXPECT_THROW(OptionsValidator::validate(this->options), std::invalid_argument)
        << "Namespaces starting with a digit must not be allowed";

    this->options.cpp_namespace = "проект";
    EXPECT_THROW(OptionsValidator::validate(this->options), std::invalid_argument)
        << "Namespaces containing non-ascii symbols must not be allowed";
}

TYPED_TEST(OptionsValidatorTest, ProjectType)
{
    this->options.type = static_cast<ProjectType>(0xCA);
    EXPECT_THROW(OptionsValidator::validate(this->options), std::invalid_argument)
        << "Type has to be a valid enum value";
}

TYPED_TEST(OptionsValidatorTest, CppStandard)
{
    this->options.standard = static_cast<CppStandard>(0xFE);
    EXPECT_THROW(OptionsValidator::validate(this->options), std::invalid_argument)
        << "C++ standard has to be a valid enum value";
}

TYPED_TEST(OptionsValidatorTest, Dependencies)
{
    static constexpr auto dependencies = {
        Dependencies{},
        Dependencies{Dependency::Catch2, Dependency::QTest},
        Dependencies{Dependency::Catch2, Dependency::GoogleTest},
        Dependencies{Dependency::GoogleTest, Dependency::QTest},
        Dependencies{Dependency::GoogleTest, Dependency::Catch2, Dependency::QTest},
    };

    for (auto entry : dependencies)
    {
        this->options.dependencies = entry;
        EXPECT_THROW(OptionsValidator::validate(this->options), std::invalid_argument)
            << "Dependencies have to include exactly one test framework";
    }
}

TEST(GenerationOptionsValidatorTest, Location)
{
    GenerationOptions options = valid_options();

    options.location = std::filesystem::path{};
    EXPECT_THROW(OptionsValidator::validate(options), std::invalid_argument) << "Location can't be empty";

    options.location = "non_existent";
    EXPECT_THROW(OptionsValidator::validate(options), std::invalid_argument) << "Location has to exist";

    options.location = "test_options_validator.exe";
    EXPECT_THROW(OptionsValidator::validate(options), std::invalid_argument) << "Location has to be the directory";

    options.name = "CMakeFiles";
    options.location = std::filesystem::current_path();
    EXPECT_THROW(OptionsValidator::validate(options), std::invalid_argument) << "Generation path has to be empty";
}
