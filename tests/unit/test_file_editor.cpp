#include "auxiliary.hpp"
#include "file_editor.hpp"
#include "gtest/gtest.h"

using namespace gena;

struct FileEditorTest : public ::testing::TestWithParam<std::pair<Options, std::filesystem::path>>
{
    FileEditorTest()
    {
        std::filesystem::copy(assets, created);
    }
    ~FileEditorTest()
    {
        std::filesystem::remove_all(created);
    }

    std::filesystem::path assets = std::filesystem::current_path() / "assets";
    std::filesystem::path created = assets / "created";
};

TEST_F(FileEditorTest, ReplaceInDirectoryName)
{
    const std::filesystem::path dir{created / "butterfly"};

    std::filesystem::create_directory(dir);
    FileEditor::replace_in_name(dir, "butter", "dragon");

    EXPECT_TRUE(std::filesystem::exists(created / "dragonfly"));
    EXPECT_FALSE(std::filesystem::exists(created / "butterfly"));
}

TEST_F(FileEditorTest, ReplaceInFileName)
{
    const std::filesystem::path file{created / "sunflower.txt"};

    std::ofstream out(file, std::ios::out);
    out.close(); // editor can't rename a file while it's open
    FileEditor::replace_in_name(file, "sun", "moon");

    EXPECT_TRUE(std::filesystem::exists(created / "moonflower.txt"));
    EXPECT_FALSE(std::filesystem::exists(created / "sunflower.txt"));
}

TEST_F(FileEditorTest, RenderTemplatesExceptions)
{
    EXPECT_THROW(FileEditor editor{Options{}}, std::invalid_argument);
}

TEST_P(FileEditorTest, RenderTemplates)
{
    auto [options, filename] = GetParam();
    FileEditor{options}.render_templates(created / "original.txt");

    const std::string actual = content_of(created / filename);
    const std::string expected = content_of(created / "original.txt");

    EXPECT_EQ(actual, expected);
}

using enum Dependency;
using enum CppStandard;
using enum ProjectType;
using namespace std::filesystem;

// clang-format off
INSTANTIATE_TEST_SUITE_P(, FileEditorTest, ::testing::Values(
    std::make_pair(Options{"first_name", library,     cpp17, googletest, current_path()}, "expected1.txt"),
    std::make_pair(Options{"SecondName", executable,  cpp20, {catch2, CLI11}, current_path()}, "expected2.txt"),
    std::make_pair(Options{"Third_Name", qmainwindow, cpp23, {qtest , spdlog}, current_path()}, "expected3.txt")));
// clang-format on
