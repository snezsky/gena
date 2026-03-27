#include <QtConcurrentRun>
#include <QFileDialog>
#include <QMessageBox>
#include <QTimer>

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

void MainWindow::on_checkBox_catch2_toggled(bool checked)
{
    if (!checked)
    {
        /* Why would you want to disable test library? */
        constexpr static std::chrono::milliseconds timeout{500};
        QTimer::singleShot(timeout, this, [this]() {
            ui->checkBox_catch2->setCheckState(Qt::Checked);
        });
    }
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
        QMessageBox::question(this, "Brah...", "Nice try, mastermind, but no.");
        QApplication::quit();
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
    catch (std::exception &e)
    {
        emit generation_failed(e.what());
        return;
    }
    emit generation_finished();
}

void MainWindow::unblock_gui_with_success_message()
{
    progressDialog_.hide();
    QMessageBox::information(this, QString(), "Generation successful!");
}

void MainWindow::unblock_gui_with_error_message(const QString &message)
{
    progressDialog_.hide();
    QMessageBox::critical(this, "Generation failed!", message);
}

gena::ProjectType MainWindow::compute_project_type() const
{
    if (ui->radioButton_exe->isChecked()) { return gena::ProjectType::exe; }
    if (ui->radioButton_lib->isChecked()) { return gena::ProjectType::lib; }

    throw std::invalid_argument("unknown project type");
}

gena::CppStandard MainWindow::compute_cpp_standard() const
{
    const QString standard = ui->comboBox_cpp_standard->currentText();

    if (standard == "c++17") { return gena::CppStandard::cpp17; }
    if (standard == "c++20") { return gena::CppStandard::cpp20; }
    if (standard == "c++23") { return gena::CppStandard::cpp23; }

    throw std::invalid_argument("unknown cpp standard");
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
