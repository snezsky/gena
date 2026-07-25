#include <QApplication>
#include "mainwindow.hpp"

int main(int argc, char *argv[]) noexcept
{
    const QApplication app(argc, argv);
    <@ namespace @>::MainWindow window;
    window.show();
    return QApplication::exec();
}
