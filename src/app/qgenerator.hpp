#pragma once

#include "generator.hpp"

namespace gena
{
    /* QML wrapper for Generator */
    class QGenerator : public QObject
    {
        Q_OBJECT
        QML_NAMED_ELEMENT(Generator)
        Q_PROPERTY(QUrl projectDirectory MEMBER project_directory_)

      public slots:
        void generate(const GenerationOptions &options);

        /* See GenerationOptions::fromQVariantMap */
        void generateAsync(const QVariantMap &options);

      signals:
        void generationFinished();
        void generationFailed(const QString &error);

      private:
        Generator generator_;
        QUrl project_directory_;
    };
} // namespace gena
