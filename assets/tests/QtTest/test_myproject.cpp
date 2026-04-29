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

    QCOMPARE(<@ project_name @>::split("", ' '), strings{});
    QCOMPARE(<@ project_name @>::split("", 'x'), strings{});
    QCOMPARE(<@ project_name @>::split("with_delimiter", '_'), strings({"with", "delimiter"}));
    QCOMPARE(<@ project_name @>::split("withoutYdelimiter", 'N'), strings({"withoutYdelimiter"}));
    QCOMPARE(<@ project_name @>::split(" beginning with", ' '), strings({"", "beginning", "with"}));
    QCOMPARE(<@ project_name @>::split("ending_with_", '_'), strings({"ending", "with"}));
    QCOMPARE(<@ project_name @>::split("=sequ==ential==", '='), strings({"", "sequ", "", "ential", ""}));
    QCOMPARE(<@ project_name @>::split("a lot of delimiters", ' '), strings({"a", "lot", "of", "delimiters"}));
}

QTEST_MAIN(StringManipulationTest)
#include "test_<@ project_name @>.moc"
