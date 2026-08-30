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

TYPED_TEST(OptionsValidatorTest, TestFramework)
{
    this->options.test_framework = static_cast<TestFramework>(0xFE);
    EXPECT_ANY_THROW(OptionsValidator::validate(this->options)) << "Test framework has to be a valid enum value";
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

TEST(GenerationOptionsValidatorTest, SubmoduleUrls)
{
    const std::vector<std::string> validUrls{
        "git@github.com:user/repo.git",
        "git@gitlab.com:group/project.git",
        "git://github.com/user/repo.git",
        "git://example.com/project.git",
        "https://github.com/user/repo",
        "https://github.com/user/repo.git",
        "https://gitlab.com/group/project.git",
        "ssh://git@github.com/user/repo.git",
        "ssh://git@example.com:2222/user/repo.git",
    };

    const std::vector<std::string> invalidUrls{
        "http//missing-colon.com/repo.git",   "https:///missing-host",
        "https://host with spaces/repo.git",  "https://:8080/repo.git",
        "https://host:invalid-port/repo.git", "ssh:///missing-host/repo.git",
        "git:///missing-host/repo.git",       "@github.com:repo.git",
        "git@@github.com:repo.git",
    };

    GenerationOptions options = valid_options();

    options.submodule_urls = {""};
    EXPECT_ANY_THROW(OptionsValidator::validate(options)) << "Submodule url can't be empty";

    options.submodule_urls = {" "};
    EXPECT_ANY_THROW(OptionsValidator::validate(options)) << "Submodule url can't contain only whitespaces";

    options.submodule_urls = {" https://github.com/snezsky/gena"};
    EXPECT_ANY_THROW(OptionsValidator::validate(options)) << "Submodule url can't start with a whitespace";

    options.submodule_urls = {"https://github.com/snezsky/gena "};
    EXPECT_ANY_THROW(OptionsValidator::validate(options)) << "Submodule url can't end with a whitespace";

    for (const auto &url : invalidUrls)
    {
        options.submodule_urls = {url};
        EXPECT_ANY_THROW(OptionsValidator::validate(options)) << "url: " << url;
    }

    for (const auto &url : validUrls)
    {
        options.submodule_urls = {url};
        EXPECT_NO_THROW(OptionsValidator::validate(options)) << "url: " << url;
    }
}
