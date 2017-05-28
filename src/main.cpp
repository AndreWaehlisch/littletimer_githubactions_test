#include "mainwindow.h"
#include "globals.h"

#include <QDesktopWidget>

// allocate global variables
const QIcon *THE_ICON = Q_NULLPTR;
QSystemTrayIcon *THE_SYSTEM_TRAY_ICON = Q_NULLPTR;
QVector<MainWindow *> *THE_WINDOWPOINTER_VECTOR = Q_NULLPTR;

int main(int argc, char *argv[]) {
    Q_INIT_RESOURCE(images); // init resources (needed for static build)
    QApplication app(argc, argv); // create the QApplication (there can only be a single one)

    const QIcon mainIcon(":/hourglass.ico");
    THE_ICON = &mainIcon;

    QVector<MainWindow *> windowPointer_vector;
    THE_WINDOWPOINTER_VECTOR = &windowPointer_vector;

    MainWindow *w = new MainWindow(); // create the first default timer window
    w->setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, w->size(), app.desktop()->availableGeometry())); // position window on screen center (https://wiki.qt.io/How_to_Center_a_Window_on_the_Screen)
    w->setWindowIcon(mainIcon); // main icon displayed on top left of main window
    w->show();

    windowPointer_vector.append(w); // add the first default timer window to our list (this assigns the ID=0)

    return app.exec();
}
