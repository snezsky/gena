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
    EXPECT_ANY_THROW(OptionsValidator::validate(this->options)) << "Empty names must not be allowed";

    this->options.name = " ";
    EXPECT_ANY_THROW(OptionsValidator::validate(this->options))
        << "Names consisting of whitespaces must not be allowed";

    this->options.name = " ";
    EXPECT_ANY_THROW(OptionsValidator::validate(this->options))
        << "Names consisting whitespaces in the middle must not be allowed";

    this->options.name = "project-name";
    EXPECT_ANY_THROW(OptionsValidator::validate(this->options)) << "Names containing '-' must not be allowed";

    this->options.name = "7project";
    EXPECT_ANY_THROW(OptionsValidator::validate(this->options)) << "Names starting with a digit must not be allowed";

    this->options.name = "проект";
    EXPECT_ANY_THROW(OptionsValidator::validate(this->options))
        << "Names containing non-ascii symbols must not be allowed";
}

TYPED_TEST(OptionsValidatorTest, CppNamespace)
{
    this->options.cpp_namespace = "";
    EXPECT_ANY_THROW(OptionsValidator::validate(this->options)) << "Empty namespaces must not be allowed";

    this->options.cpp_namespace = " ";
    EXPECT_ANY_THROW(OptionsValidator::validate(this->options))
        << "Namespaces consisting of whitespaces must not be allowed";

    this->options.cpp_namespace = " ";
    EXPECT_ANY_THROW(OptionsValidator::validate(this->options))
        << "Namespaces consisting whitespaces in the middle must not be allowed";

    this->options.cpp_namespace = "namespace-name";
    EXPECT_ANY_THROW(OptionsValidator::validate(this->options)) << "Namespaces containing '-' must not be allowed";

    this->options.cpp_namespace = "7namespace";
    EXPECT_ANY_THROW(OptionsValidator::validate(this->options))
        << "Namespaces starting with a digit must not be allowed";

    this->options.cpp_namespace = "проект";
    EXPECT_ANY_THROW(OptionsValidator::validate(this->options))
        << "Namespaces containing non-ascii symbols must not be allowed";
}

TYPED_TEST(OptionsValidatorTest, ProjectType)
{
    this->options.type = static_cast<ProjectType>(0xCA);
    EXPECT_ANY_THROW(OptionsValidator::validate(this->options)) << "Type has to be a valid enum value";
}

TYPED_TEST(OptionsValidatorTest, CppStandard)
{
    this->options.standard = static_cast<CppStandard>(0xFE);
    EXPECT_ANY_THROW(OptionsValidator::validate(this->options)) << "C++ standard has to be a valid enum value";
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
        EXPECT_ANY_THROW(OptionsValidator::validate(this->options))
            << "Dependencies have to include exactly one test framework";
    }
}

TEST(GenerationOptionsValidatorTest, OutputDirectory)
{
    GenerationOptions options = valid_options();

    options.output_directory = std::filesystem::path{};
    EXPECT_ANY_THROW(OptionsValidator::validate(options)) << "Output directory can't be empty";

    options.output_directory = "non_existent";
    EXPECT_ANY_THROW(OptionsValidator::validate(options)) << "Output directory has to exist";

    options.output_directory = "test_options_validator.exe";
    EXPECT_ANY_THROW(OptionsValidator::validate(options)) << "Output directory has to be the directory";

    options.name = "CMakeFiles";
    options.output_directory = std::filesystem::current_path();
    EXPECT_ANY_THROW(OptionsValidator::validate(options)) << "Generation path has to be empty";
}
