#include "git_client.hpp"
#include "gtest/gtest.h"

using gena::GitClient;

TEST(GitClientTest, RepositoryName)
{
    EXPECT_EQ(GitClient::repository_name("https://github.com/user/repo"), "repo");
    EXPECT_EQ(GitClient::repository_name("https://github.com/user/repo.git"), "repo");
    EXPECT_EQ(GitClient::repository_name("ssh://git@example.com:2222/user/repo.git"), "repo");
    EXPECT_EQ(GitClient::repository_name("git@github.com:user/repo.git"), "repo");
}

TEST(GitClientTest, RepositoryNameIgnoresTrailingSlash)
{
    EXPECT_EQ(GitClient::repository_name("https://github.com/user/repo/"), "repo");
    EXPECT_EQ(GitClient::repository_name("https://github.com/user/repo.git/"), "repo");
    EXPECT_EQ(GitClient::repository_name("https://github.com/user/repo//"), "repo");
}

TEST(GitClientTest, RepositoryNameOfScpUrlWithoutSlash)
{ EXPECT_EQ(GitClient::repository_name("git@host:repo.git"), "repo"); }

TEST(GitClientTest, RepositoryNameIsEmptyWithoutName)
{
    EXPECT_TRUE(GitClient::repository_name("https://github.com/user/.git").empty());
    EXPECT_TRUE(GitClient::repository_name("").empty());
}
