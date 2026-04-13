#include <QFileDialog>
#include <QMessageBox>
#include <QtConcurrentRun>

#include "generator.hpp"
#include "mainwindow.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow()),
      progressDialog_("Generation...", QString(), 0, 0, this),
      nameValidator_(QRegularExpression("^[A-Za-z][A-Za-z0-9]*(?:_[A-Za-z0-9]+)*$"))
{
    ui->setupUi(this);
    ui->lineEdit_name->setValidator(&nameValidator_);

    progressDialog_.cancel();
    progressDialog_.setCancelButton(nullptr);
    progressDialog_.setWindowModality(Qt::WindowModal);
    progressDialog_.setWindowFlag(Qt::WindowCloseButtonHint, false);
    progressDialog_.setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint);

    connect(this, &MainWindow::generation_failed, this, &MainWindow::unblock_gui_with_error_message);
    connect(this, &MainWindow::generation_finished, this, &MainWindow::unblock_gui_with_success_message);
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

    const QString location = QFileDialog::getExistingDirectory(this, "Project destination", "");
    if (location.isEmpty())
    {
        /* The user closed the file dialog */
        return;
    }

    const gena::Options options{.name = ui->lineEdit_name->text().toStdString(),
                                .type = compute_project_type(),
                                .standard = compute_cpp_standard(),
                                .dependencies = compute_dependencies(),
                                .location = location.toStdString()};
    progressDialog_.show();
    std::ignore = QtConcurrent::run(&MainWindow::generate, this, options);
}

void MainWindow::generate(const gena::Options &options)
{
    try
    {
        gena::Generator::generate(options);
    }
    catch (const std::exception &e)
    {
        emit generation_failed(e.what());
        return;
    }
    emit generation_finished();
}

void MainWindow::unblock_gui_with_success_message()
{
    progressDialog_.hide();
    QMessageBox::information(this, " ", "Generation successful!");
}

void MainWindow::unblock_gui_with_error_message(const QString &message)
{
    progressDialog_.hide();
    QMessageBox::critical(this, "Generation failed!", message);
}

gena::ProjectType MainWindow::compute_project_type() const
{
    const QString projectType = ui->comboBox_project_type->currentText();

    if (projectType == "Library") { return gena::ProjectType::library; }
    if (projectType == "Executable") { return gena::ProjectType::executable; }
    if (projectType == "QMainWindow") { return gena::ProjectType::qmainwindow; }

    throw std::invalid_argument("Unknown project type");
}

gena::CppStandard MainWindow::compute_cpp_standard() const
{
    const QString standard = ui->comboBox_cpp_standard->currentText();

    if (standard == "c++17") { return gena::CppStandard::cpp17; }
    if (standard == "c++20") { return gena::CppStandard::cpp20; }
    if (standard == "c++23") { return gena::CppStandard::cpp23; }

    throw std::invalid_argument("Unknown cpp standard");
}

gena::Dependencies MainWindow::compute_dependencies() const
{
    gena::Dependencies deps;

    if (ui->checkBox_json->isChecked()) { deps |= gena::Dependency::json; }
    if (ui->checkBox_CLI11->isChecked()) { deps |= gena::Dependency::CLI11; }
    if (ui->checkBox_spdlog->isChecked()) { deps |= gena::Dependency::spdlog; }

    if (ui->comboBox_test_framework->currentText() == "QtTest") { deps |= gena::Dependency::qtest; }
    else if (ui->comboBox_test_framework->currentText() == "Catch2") { deps |= gena::Dependency::catch2; }
    else if (ui->comboBox_test_framework->currentText() == "GoogleTest") { deps |= gena::Dependency::googletest; }
    else { throw std::invalid_argument("Unknown test framework"); }

    return deps;
}
