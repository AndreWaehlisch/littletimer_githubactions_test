#include "mainwindow.h"
#include <QApplication>
#include <QStyle>
#include <QDesktopWidget>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow w;

    w.setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, w.size(), app.desktop()->availableGeometry())); // position window on screen center (https://wiki.qt.io/How_to_Center_a_Window_on_the_Screen)
    const QIcon *mainIcon = new QIcon(":/bell.ico");
    w.setWindowIcon(*mainIcon); // main icon displayed on top left of main window
    w.show();

    return app.exec();
}
