#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "auxiliary.hpp"
#include "generation_options_to_strings.hpp"
#include "generator.hpp"

namespace
{
    class MockGitClient : public gena::IGitClient
    {
      public:
        virtual ~MockGitClient() = default;
        MOCK_METHOD(void, set_repository_path, (std::filesystem::path), (override));
        MOCK_METHOD(void, init, (), (override));
        MOCK_METHOD(void, add, (const std::string &), (override));
        MOCK_METHOD(void, commit, (const std::string &), (override));
        MOCK_METHOD(void, add_submodule, (const std::string &), (override));
        MOCK_METHOD(void, add_submodules, (const std::vector<std::string> &), (override));
        MOCK_METHOD(void, set_execute_permission, (const std::filesystem::path &), (override));
    };
    using NiceMockGitClient = ::testing::NiceMock<MockGitClient>;

    class MockAssetsEnvironment : public ::testing::Environment
    {
      public:
        MockAssetsEnvironment()
        {
            static const std::string qtest{gena::to_string(gena::TestFramework::QTest)};
            static const std::string library{gena::to_string(gena::ProjectType::Library)};

            std::filesystem::create_directories("assets/test_frameworks");
            std::filesystem::create_directories("assets/tests/" + qtest);
            std::filesystem::create_directories("assets/type/Library/" + library);
            std::filesystem::create_directories("assets/common/scripts");

            std::ofstream cmake("assets/test_frameworks/CMakeLists.txt");
            std::ofstream cover("assets/common/scripts/coverage.sh");
        }
        ~MockAssetsEnvironment() override
        { std::filesystem::remove_all("assets"); }
    };
    const auto *env = testing::AddGlobalTestEnvironment(new MockAssetsEnvironment);
} // namespace

TEST(TestGenerator, GitFunctionsCalled)
{
    using ::testing::_;

    auto mockGitClient = std::make_unique<NiceMockGitClient>();
    EXPECT_CALL(*mockGitClient, set_repository_path(_)).Times(1);
    EXPECT_CALL(*mockGitClient, init()).Times(1);
    EXPECT_CALL(*mockGitClient, add(_)).Times(2);
    EXPECT_CALL(*mockGitClient, commit(_)).Times(1);
    EXPECT_CALL(*mockGitClient, add_submodules(_)).Times(1);
    EXPECT_CALL(*mockGitClient, set_execute_permission(_)).Times(1);

    gena::Generator generator;
    generator.generate(gena::valid_options(), std::move(mockGitClient));
}

TEST(TestGenerator, WorksOutsideApplicationDir)
{
    auto cwd = std::filesystem::current_path();
    std::filesystem::current_path(cwd.parent_path());

    gena::Generator generator;
    generator.generate(gena::valid_options(), std::make_unique<NiceMockGitClient>());

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
    generator.generate(gena::valid_options(), std::make_unique<NiceMockGitClient>());

    EXPECT_ANY_THROW(generator.generate(gena::options_with_invalid_standard(), std::make_unique<NiceMockGitClient>()));
    EXPECT_ANY_THROW(generator.project_directory());
}

TEST(TestGenerator, QtTestDoesNotCreateDepsDirectory)
{
    gena::GenerationOptions options = gena::valid_options();
    options.test_framework = gena::TestFramework::QTest;

    gena::Generator generator;
    generator.generate(options, std::make_unique<NiceMockGitClient>());

    EXPECT_FALSE(std::filesystem::exists(generator.project_directory() / "deps"));
}

TEST(TestGenerator, DoesNotCreateDirectoryIfOptionsInvalid)
{
    gena::Generator generator;
    gena::GenerationOptions options = gena::options_with_invalid_standard();

    EXPECT_ANY_THROW(generator.generate(options, std::make_unique<NiceMockGitClient>()));
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

    EXPECT_ANY_THROW(generator.generate(options, std::make_unique<NiceMockGitClient>()));
    EXPECT_TRUE(std::filesystem::exists(projectDir));
    EXPECT_TRUE(std::filesystem::exists(projectDir / file));
}
