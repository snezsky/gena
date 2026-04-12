#include "file_editor.hpp"

#include <QDir>
#include <QFileInfo>

namespace gena
{
    void FileEditor::replace_in_name(QFile &file, const QString &before, const QString &after)
    {
        const QFileInfo info{file};
        if (!info.fileName().contains(before)) { return; }

        const QDir path{info.absolutePath()};
        const QString newName{info.fileName().replace(before, after)};

        if (!file.rename(path.filePath(newName)))
        {
            throw std::filesystem::filesystem_error("Failed to rename file " + newName.toStdString(),
                                                    std::make_error_code(std::errc::io_error));
        }
    }

    void FileEditor::replace_in_content(QFile &file, const QString &before, const QString &after)
    {
        static const QList extensions{"txt", "cmake", "json", "md", "rc", "hpp", "cpp"};

        const QFileInfo info{file};
        if (!info.isFile()) { return; }
        if (!extensions.contains(info.suffix())) { return; }

        if (!file.open(QIODevice::ReadWrite))
        {
            throw std::filesystem::filesystem_error("Can't open " + file.fileName().toStdString(),
                                                    std::make_error_code(std::errc::io_error));
        }

        QString content = file.readAll();
        content.replace(before, after);

        file.resize(0);
        file.seek(0);
        file.write(content.toUtf8());
        file.close();
    }
} // namespace gena
