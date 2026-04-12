#include "file_editor.hpp"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <QTemporaryFile>

using namespace gena;

namespace
{
    struct FileEditorTest : testing::Test
    {
        static std::string replaced(const QString &original, const QString &from, const QString &to)
        {
            QTemporaryFile file("XXXXXX.txt");
            if (!file.open()) { return "Failed to open file"; }
            file.write(original.toUtf8());
            file.close();

            FileEditor::replace_in_content(file, from, to);

            if (!file.open()) { return "Failed to open file"; }
            return file.readAll().toStdString();
        }

        static std::string renamed(const QString &original, const QString &from, const QString &to)
        {
            QTemporaryFile file(original);
            if (!file.open()) { return "Failed to open file"; }
            FileEditor::replace_in_name(file, from, to);

            if (!file.open()) { return "Failed to open file"; }
            return file.fileName().toStdString();
        }
    };
} // namespace

TEST_F(FileEditorTest, ReplaceInName)
{
    EXPECT_THAT(renamed("coco", "co", "do"), testing::HasSubstr("dodo"));
    EXPECT_THAT(renamed("works", "s", "ing"), testing::HasSubstr("working"));
    EXPECT_THAT(renamed("UpperCase", "case", "cuts"), testing::HasSubstr("UpperCase"));
    EXPECT_THAT(renamed("lowercase", "Lower", "stair"), testing::HasSubstr("lowercase"));

    QFile closed("closed_file");
    EXPECT_THROW(FileEditor::replace_in_name(closed, "closed", "open"), std::filesystem::filesystem_error);
}

TEST_F(FileEditorTest, ReplaceInContent)
{
    EXPECT_STREQ(replaced("", "", "").c_str(), "");
    EXPECT_STREQ(replaced("coco", "co", "do").c_str(), "dodo");
    EXPECT_STREQ(replaced("works", "s", "ing").c_str(), "working");
    EXPECT_STREQ(replaced("UpperCase", "case", "cuts").c_str(), "UpperCase");
    EXPECT_STREQ(replaced("lowercase", "Lower", "stair").c_str(), "lowercase");
    EXPECT_STREQ(replaced("SpecialSymb0l&", "Symb0l&", "Symbols").c_str(), "SpecialSymbols");
}
