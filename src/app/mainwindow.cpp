#include <QFileDialog>
#include <QMainWindow>
#include <QMessageBox>
#include <QTimer>

#include "generator.hpp"
#include "mainwindow.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow()),
      nameValidator_(QRegularExpression("^[A-Za-z][A-Za-z0-9]*(?:_[A-Za-z0-9]+)*$"))
{
    ui->setupUi(this);
    ui->lineEdit_name->setValidator(&nameValidator_);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_generate_clicked()
{
    if (ui->lineEdit_name->text().isEmpty())
    {
        QMessageBox::warning(this, "Oops...", "Can't create a project without a name!");
        return;
    }

    if (!ui->checkBox_catch2->isChecked())
    {
        QMessageBox::question(this, "Brah...", "You are the smart one, huh?");
        QApplication::quit();
        return;
    }

    const QString location = QFileDialog::getExistingDirectory(this, "Project destination", "");
    if (location.isEmpty())
    {
        /* The user closed the file dialog */
        return;
    }

    try
    {
        const gena::Options options{.name = ui->lineEdit_name->text().toStdString(),
                                    .type = compute_project_type(),
                                    .standard = compute_cpp_standard(),
                                    .dependencies = compute_dependencies(),
                                    .location = location.toStdString()};
        gena::Generator::generate(options);
    }
    catch (std::exception &e)
    {
        QMessageBox::critical(this, "Generation failed!", e.what());
        return;
    }

    QMessageBox::information(this, " ", "Generation successful!");
}

void MainWindow::on_checkBox_catch2_toggled(bool checked)
{
    if (!checked)
    {
        /* Why would you want to disable test library? */
        QTimer::singleShot(std::chrono::seconds(1), this, [this]() {
            ui->checkBox_catch2->setCheckState(Qt::Checked);
        });
    }
}

gena::ProjectType MainWindow::compute_project_type() const
{
    if (ui->radioButton_exe->isChecked()) { return gena::ProjectType::exe; }
    if (ui->radioButton_lib->isChecked()) { return gena::ProjectType::lib; }

    [[unlikely]] throw std::invalid_argument("unknown project type");
}

gena::CppStandard MainWindow::compute_cpp_standard() const
{
    const QString standard = ui->comboBox_cpp_standard->currentText();

    if (standard == "c++17") { return gena::CppStandard::cpp17; }
    if (standard == "c++20") { return gena::CppStandard::cpp20; }
    if (standard == "c++23") { return gena::CppStandard::cpp23; }

    [[unlikely]] throw std::invalid_argument("unknown cpp standard");
}

gena::Dependencies MainWindow::compute_dependencies() const
{
    gena::Dependencies deps;

    if (ui->checkBox_json->isChecked()) { deps |= gena::Dependency::json; }
    if (ui->checkBox_CLI11->isChecked()) { deps |= gena::Dependency::CLI11; }
    if (ui->checkBox_spdlog->isChecked()) { deps |= gena::Dependency::spdlog; }
    if (ui->checkBox_catch2->isChecked()) { deps |= gena::Dependency::catch2; }

    return deps;
}
