#include <gtest/gtest.h>

#include "auxiliary.hpp"
#include "generation_options_to_strings.hpp"
#include "generator.hpp"

namespace
{
    class MocksAssetsEnvironment : public ::testing::Environment
    {
      public:
        MocksAssetsEnvironment()
        {
            static const std::string qtest{gena::to_string(gena::Dependency::QTest)};
            static const std::string library{gena::to_string(gena::ProjectType::Library)};

            std::filesystem::create_directories("assets/deps");
            std::filesystem::create_directories("assets/tests/" + qtest);
            std::filesystem::create_directories("assets/type/Library/" + library);
            std::filesystem::create_directories("assets/common/scripts");

            std::ofstream cmake("assets/deps/CMakeLists.txt");
            std::ofstream cover("assets/common/scripts/coverage.sh");
        }
        ~MocksAssetsEnvironment() override
        { std::filesystem::remove_all("assets"); }
    };
    const auto *env = testing::AddGlobalTestEnvironment(new MocksAssetsEnvironment);
} // namespace

TEST(TestGenerator, WorksOutsideApplicationDir)
{
    auto cwd = std::filesystem::current_path();
    std::filesystem::current_path(cwd.parent_path());

    gena::Generator generator;
    generator.generate(gena::valid_options());

    std::filesystem::current_path(cwd);
}

TEST(TestGenerator, ProjectDirectoryThrowsWithoutGeneration)
{
    gena::Generator generator;
    EXPECT_ANY_THROW(generator.project_directory());
}

TEST(TestGenerator, ProjectDirectoryClearsOldValue)
{
    gena::Generator generator;
    generator.generate(gena::valid_options());

    EXPECT_ANY_THROW(generator.generate(gena::options_with_invalid_standard()));
    EXPECT_ANY_THROW(generator.project_directory());
}

TEST(TestGenerator, GitRepoCreatedIfOptionSet)
{
    gena::GenerationOptions options = gena::valid_options();
    options.setup_git = true;

    gena::Generator generator;
    generator.generate(options);

    EXPECT_TRUE(std::filesystem::exists(generator.project_directory() / ".git"));
}

TEST(TestGenerator, GitRepoNotCreatedIfOptionNotSet)
{
    gena::GenerationOptions options = gena::valid_options();
    options.setup_git = false;

    gena::Generator generator;
    generator.generate(options);

    EXPECT_FALSE(std::filesystem::exists(generator.project_directory() / ".git"));
}

TEST(TestGenerator, SingleQtTestDoesNotCreateDepsDirectory)
{
    gena::GenerationOptions options = gena::valid_options();
    options.dependencies = gena::Dependency::QTest;

    gena::Generator generator;
    generator.generate(options);

    EXPECT_FALSE(std::filesystem::exists(generator.project_directory() / "deps"));
}

TEST(TestGenerator, DoesNotCreateDirectoryIfOptionsInvalid)
{
    gena::Generator generator;
    gena::GenerationOptions options = gena::options_with_invalid_standard();

    EXPECT_ANY_THROW(generator.generate(options));
    EXPECT_FALSE(std::filesystem::exists(gena::would_be_project_directory(options)));
}

TEST(TestGenerator, DoesNotModifyExistingDirectoryIfOptionsInvalid)
{
    const gena::GenerationOptions options = gena::options_with_invalid_standard();
    const std::filesystem::path projectDir = gena::would_be_project_directory(options);
    const std::filesystem::path file = "exists.txt";

    std::filesystem::create_directory(projectDir);
    std::ofstream fileToExist(projectDir / file);

    gena::Generator generator;

    EXPECT_ANY_THROW(generator.generate(options));
    EXPECT_TRUE(std::filesystem::exists(projectDir));
    EXPECT_TRUE(std::filesystem::exists(projectDir / file));
}
