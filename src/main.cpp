#include "mainwindow.h"
#include <QApplication>
#include <QStyle>
#include <QDesktopWidget>
#include "simpletimer.h"

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(images); // init resources (needed for static build)
    QApplication app(argc, argv);
    MainWindow w1;
    MainWindow w2;
    const QIcon mainIcon(":/bell.ico");

    w1.setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, w1.size(), app.desktop()->availableGeometry())); // position window on screen center (https://wiki.qt.io/How_to_Center_a_Window_on_the_Screen)
    w1.setWindowIcon(mainIcon); // main icon displayed on top left of main window
    w1.show();

    w2.setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, w2.size(), app.desktop()->availableGeometry())); // position window on screen center (https://wiki.qt.io/How_to_Center_a_Window_on_the_Screen)
    w2.setWindowIcon(mainIcon); // main icon displayed on top left of main window
    w2.show();

    return app.exec();
}
