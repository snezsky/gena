#include <QTest>
#include "myproject.hpp"

class StringManipulationTest : public QObject
{
   Q_OBJECT

  private slots:
    void splitting_checks() const;
};

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

QTEST_MAIN(StringManipulationTest)
#include "test_myproject.moc"
