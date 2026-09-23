#include "mainwindow.hpp"
#include "ui_mainwindow.h"

namespace <@ namespace @>
{
    MainWindow::MainWindow(QWidget* parent)
        : QMainWindow(parent),
          ui(std::make_unique<Ui::MainWindow>())
    { ui->setupUi(this); }

    MainWindow::~MainWindow() = default;
} // namespace <@ namespace @>
