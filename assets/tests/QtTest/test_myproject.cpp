#include "<@ project_name @>/<@ project_name @>.hpp"
#include <QTest>

class Test<@ capitalize(project_name) @> : public QObject
{
    Q_OBJECT
};

QTEST_MAIN(Test<@ capitalize(project_name) @>)
#include "test_<@ project_name @>.moc"
