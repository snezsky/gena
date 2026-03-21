#include <QFile>
#include <QString>
#include <QRegularExpression>

namespace gena
{
    struct FileEditor
    {
        static void replace_in_name(QFile &file, const QString &before, const QString &after);

        static void replace_in_content(QFile &file, const QString &before, const QString &after);

        static void remove_lines(QFile &file, const QRegularExpression &regexp);
    };

} // namespace gena
