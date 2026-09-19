#include "<@ project_name @>/<@ project_name @>.hpp"
#include <QTest>

class Test<@ capitalized_name @> : public QObject
{
    Q_OBJECT
};

QTEST_MAIN(Test<@ capitalized_name @>)
#include "test_<@ project_name @>.moc"
