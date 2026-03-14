#pragma once

#include <filesystem>

#include <QMainWindow>
#include <QRegularExpressionValidator>

#include "generator.hpp"


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

  private slots:
    void on_pushButton_generate_clicked();
    void on_checkBox_catch2_toggled(bool checked);

  private:
    [[nodiscard]] gena::ProjectType compute_project_type() const;
    [[nodiscard]] gena::CppStandard compute_cpp_standard() const;
    [[nodiscard]] gena::Dependencies compute_dependencies() const;

  private:
    Ui::MainWindow *ui;

    QRegularExpressionValidator nameValidator_;
};
