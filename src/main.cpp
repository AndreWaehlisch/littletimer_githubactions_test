#include "mainwindow.h"

#include <QDesktopWidget>

// allocate global variables
const QIcon *THE_ICON = Q_NULLPTR;

int main(int argc, char *argv[]) {
    Q_INIT_RESOURCE(images); // init resources (needed for static build)
    QApplication app(argc, argv); // create the QApplication (there can only be a single one)
    app.setQuitOnLastWindowClosed(false);  // if all windows (timers) are hidden because they are minimized to tray and then one timer is closed, then the app would close without this

    const QIcon mainIcon(":/hourglass.ico");
    THE_ICON = &mainIcon;

    MainWindow *w = new MainWindow(); // create the first default timer window (gets auto deleted)
    w->show();

    return app.exec();
}
