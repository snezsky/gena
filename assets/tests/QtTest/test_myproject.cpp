#include <QTest>
#include "<@ project_name @>/<@ project_name @>.hpp"

class StringManipulationTest : public QObject
{
   Q_OBJECT

  private slots:
    void splitting_checks() const;
};

void StringManipulationTest::splitting_checks() const
{
    using strings = std::vector<std::string>;

    QCOMPARE(<@ namespace @>::split("", ' '), strings{});
    QCOMPARE(<@ namespace @>::split("", 'x'), strings{});
    QCOMPARE(<@ namespace @>::split("with_delimiter", '_'), strings({"with", "delimiter"}));
    QCOMPARE(<@ namespace @>::split("withoutYdelimiter", 'N'), strings({"withoutYdelimiter"}));
    QCOMPARE(<@ namespace @>::split(" beginning with", ' '), strings({"", "beginning", "with"}));
    QCOMPARE(<@ namespace @>::split("ending_with_", '_'), strings({"ending", "with"}));
    QCOMPARE(<@ namespace @>::split("=sequ==ential==", '='), strings({"", "sequ", "", "ential", ""}));
    QCOMPARE(<@ namespace @>::split("a lot of delimiters", ' '), strings({"a", "lot", "of", "delimiters"}));
}

QTEST_MAIN(StringManipulationTest)
#include "test_<@ project_name @>.moc"
