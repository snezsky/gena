#include "mainwindow.hpp"
#include "ui_mainwindow.h"

namespace <@ namespace @>
{
    MainWindow::MainWindow(QWidget* parent)
        : QMainWindow(parent),
          ui(new Ui::MainWindow())
    { ui->setupUi(this); }

    MainWindow::~MainWindow()
    { delete ui; }
} // namespace <@ namespace @>
