#include <QTest>
#include "myproject.hpp"

class StringManipulationTest : public QObject
{
   Q_OBJECT

  private slots:
    void trimming_checks() const;
    void splitting_checks() const;
    void joining_checks() const;
};

void StringManipulationTest::trimming_checks() const
{
    QCOMPARE(myproject::trim(""), "");
    QCOMPARE(myproject::trim(" "), "");
    QCOMPARE(myproject::trim(" spaces "), "spaces");
    QCOMPARE(myproject::trim("nospace"), "nospace");
    QCOMPARE(myproject::trim(" leftspace"), "leftspace");
    QCOMPARE(myproject::trim("rightspace "), "rightspace");
    QCOMPARE(myproject::trim("    moreleftspaces "), "moreleftspaces");
    QCOMPARE(myproject::trim("morerightspaces    "), "morerightspaces");
}

void StringManipulationTest::splitting_checks() const
{
    using strings = std::vector<std::string>;

    QCOMPARE(myproject::split("", ' '), strings{});
    QCOMPARE(myproject::split("", 'x'), strings{});
    QCOMPARE(myproject::split("with_delimiter", '_'), strings({"with", "delimiter"}));
    QCOMPARE(myproject::split("withoutYdelimiter", 'N'), strings({"withoutYdelimiter"}));
    QCOMPARE(myproject::split(" beginning with", ' '), strings({"", "beginning", "with"}));
    QCOMPARE(myproject::split("ending_with_", '_'), strings({"ending", "with"}));
    QCOMPARE(myproject::split("=sequ==ential==", '='), strings({"", "sequ", "", "ential", ""}));
    QCOMPARE(myproject::split("a lot of delimiters", ' '), strings({"a", "lot", "of", "delimiters"}));
}

void StringManipulationTest::joining_checks() const
{
    QCOMPARE(myproject::join({""}, ""), "");
    QCOMPARE(myproject::join({"two str", "ings"}, ""), "two strings");
    QCOMPARE(myproject::join({"two", "strings"}, " "), "two strings");
    QCOMPARE(myproject::join({"", "string", ""}, ""), "string");
    QCOMPARE(myproject::join({"", "string", ""}, "__"), "__string__");
}

QTEST_MAIN(StringManipulationTest)
#include "test_myproject.moc"
