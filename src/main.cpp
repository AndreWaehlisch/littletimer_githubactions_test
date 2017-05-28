#include "mainwindow.h"
#include "globals.h"

#include <QDesktopWidget>

// allocate global variables
const QIcon *THE_ICON = Q_NULLPTR;
unsigned int THE_WINDOW_ID = 0;

int main(int argc, char *argv[]) {
    Q_INIT_RESOURCE(images); // init resources (needed for static build)
    QApplication app(argc, argv); // create the QApplication (there can only be a single one)

    const QIcon mainIcon(":/hourglass.ico");
    THE_ICON = &mainIcon;

    MainWindow *w = new MainWindow(); // create the first default timer window
    w->setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, w->size(), app.desktop()->availableGeometry())); // position window on screen center (https://wiki.qt.io/How_to_Center_a_Window_on_the_Screen)
    w->setWindowIcon(mainIcon); // main icon displayed on top left of main window
    w->show();

    return app.exec();
}
