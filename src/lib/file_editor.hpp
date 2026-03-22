#include <QFile>
#include <QString>

namespace gena
{
    struct FileEditor
    {
        static void replace_in_name(QFile &file, const QString &before, const QString &after);

        static void replace_in_content(QFile &file, const QString &before, const QString &after);
    };

} // namespace gena
