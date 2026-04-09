#pragma once

#include <QMainWindow>
#include <QProgressDialog>
#include <QRegularExpressionValidator>

#include "options.hpp"

namespace Ui
{
    class MainWindow;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

  signals:
    void generation_finished();
    void generation_failed(const QString &message);

  private slots:
    void on_pushButton_generate_clicked();

    void unblock_gui_with_success_message();
    void unblock_gui_with_error_message(const QString &message);

  private:
    void generate(const gena::Options &options);

    [[nodiscard]] gena::ProjectType compute_project_type() const;
    [[nodiscard]] gena::CppStandard compute_cpp_standard() const;
    [[nodiscard]] gena::Dependencies compute_dependencies() const;

  private:
    Ui::MainWindow *ui;

    QProgressDialog progressDialog_;
    QRegularExpressionValidator nameValidator_;
};
