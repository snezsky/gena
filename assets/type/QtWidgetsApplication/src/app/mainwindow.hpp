#pragma once

#include <memory>
#include <QMainWindow>

namespace Ui
{
    class MainWindow;
}

namespace <@ namespace @>
{
    class MainWindow : public QMainWindow
    {
        Q_OBJECT

      public:
        explicit MainWindow(QWidget *parent = nullptr);
        ~MainWindow() override;

      private:
        std::unique_ptr<Ui::MainWindow> ui;
    };
} // namespace <@ namespace @>
