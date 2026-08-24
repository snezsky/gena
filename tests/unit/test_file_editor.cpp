#include "auxiliary.hpp"
#include "file_editor.hpp"
#include "gtest/gtest.h"

using namespace gena;

struct FileEditorTest : public ::testing::TestWithParam<std::pair<GenerationOptions, std::filesystem::path>>
{
    FileEditorTest()
    { std::filesystem::copy(test_data, test_output); }

    std::filesystem::path test_output = gena::temp_directory();
    std::filesystem::path test_data = std::filesystem::current_path() / "test_data";
};

TEST_F(FileEditorTest, ReplaceInDirectoryName)
{
    const std::filesystem::path dir{test_output / "butterfly"};

    std::filesystem::create_directory(dir);
    FileEditor::replace_in_name(dir, "butter", "dragon");

    EXPECT_TRUE(std::filesystem::exists(test_output / "dragonfly"));
    EXPECT_FALSE(std::filesystem::exists(test_output / "butterfly"));
}

TEST_F(FileEditorTest, ReplaceInFileName)
{
    const std::filesystem::path file{test_output / "sunflower.txt"};

    std::ofstream out(file);
    out.close(); // editor can't rename a file while it's open
    FileEditor::replace_in_name(file, "sun", "moon");

    EXPECT_TRUE(std::filesystem::exists(test_output / "moonflower.txt"));
    EXPECT_FALSE(std::filesystem::exists(test_output / "sunflower.txt"));
}

TEST_F(FileEditorTest, ThrowsWithInvalidOptions)
{ EXPECT_ANY_THROW(FileEditor editor{GenerationOptions{}}); }

TEST_P(FileEditorTest, RenderTemplates)
{
    auto [options, filename] = GetParam();
    FileEditor{options}.render_templates(test_output / "original.txt");

    const std::string actual = content_of(test_output / "original.txt");
    const std::string expected = content_of(test_output / filename);

    EXPECT_EQ(actual, expected);
}

using enum Dependency;
using enum CppStandard;
using enum ProjectType;
using namespace std::filesystem;

// clang-format off
INSTANTIATE_TEST_SUITE_P(, FileEditorTest, ::testing::Values(
    std::pair(GenerationOptions{"first_name", Library,     Cpp17, GoogleTest, "first_namespace", current_path()}, "expected1.txt"),
    std::pair(GenerationOptions{"SecondName", ConsoleApplication,  Cpp20, {Catch2, CLI11}, "SecondNamespace", current_path()}, "expected2.txt"),
    std::pair(GenerationOptions{"Third_Name", QtWidgetsApplication, Cpp23, {QTest , Spdlog}, "Third_Namespace", current_path()}, "expected3.txt")));
// clang-format on
