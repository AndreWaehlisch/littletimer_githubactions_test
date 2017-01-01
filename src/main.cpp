#include <QDesktopWidget>
#include "mainwindow.h"

QVector<MainWindow *> MainWindow::windowPointer_vector;

int main(int argc, char *argv[]) {
    Q_INIT_RESOURCE(images); // init resources (needed for static build)
    QApplication app(argc, argv); // create the QApplication (there can only be a single one)
    MainWindow *w = new MainWindow(); // create the first default timer window
    const QIcon mainIcon(":/bell.ico");

    w->windowPointer_vector.append(w); // add the first default timer window to our list (this assigns the ID=0)
    w->setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, w->size(), app.desktop()->availableGeometry())); // position window on screen center (https://wiki.qt.io/How_to_Center_a_Window_on_the_Screen)
    w->setWindowIcon(mainIcon); // main icon displayed on top left of main window
    w->show();

    return app.exec();
}
