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

INSTANTIATE_TEST_SUITE_P(
    , FileEditorTest,
    ::testing::Values(std::pair(GenerationOptions{.name = "first_name",
                                                  .type = ProjectType::Library,
                                                  .standard = CppStandard::Cpp17,
                                                  .test_framework = TestFramework::GoogleTest,
                                                  .cpp_namespace = "first_namespace",
                                                  .output_directory = std::filesystem::current_path(),
                                                  .submodule_urls = {{"https://github.com/google/googletest"}}},
                                "expected1.txt"),
                      std::pair(GenerationOptions{.name = "SecondName",
                                                  .type = ProjectType::ConsoleApplication,
                                                  .standard = CppStandard::Cpp20,
                                                  .test_framework = TestFramework::Catch2,
                                                  .cpp_namespace = "SecondNamespace",
                                                  .output_directory = std::filesystem::current_path(),
                                                  .submodule_urls = {}},
                                "expected2.txt"),
                      std::pair(GenerationOptions{.name = "Third_Name",
                                                  .type = ProjectType::QtWidgetsApplication,
                                                  .standard = CppStandard::Cpp23,
                                                  .test_framework = TestFramework::QTest,
                                                  .cpp_namespace = "Third_Namespace",
                                                  .output_directory = std::filesystem::current_path(),
                                                  .submodule_urls = {{"https://github.com/fmtlib/fmt"}}},
                                "expected3.txt")));
