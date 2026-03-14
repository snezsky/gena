#include <QApplication>
#include "mainwindow.hpp"

int main(int argc, char *argv[]) noexcept
{
    const QApplication app(argc, argv);
    MainWindow window;
    window.show();
    return QApplication::exec();
}
