#include "mainwindow.h"

int main(int argc, char *argv[]) {
	Q_INIT_RESOURCE(images); // init resources (needed for static build)
	QApplication app(argc, argv); // create the QApplication (there can only be a single one)
	app.setQuitOnLastWindowClosed(false);  // if all windows (timers) are hidden because they are minimized to tray and then one timer is closed, then the app would close without this

	MainWindow::theIcon = QIcon(":/hourglass.ico");

	QStringList arguments = app.arguments();

	// create the first default timer window (gets auto deleted)
	MainWindow *w = new MainWindow();
	if ( arguments.size() > 1 ) {
		w->setWindowTitle(arguments.mid(1).join(' ')); // build the window title from command line arguments (except the 0th, i.e, the program name)
	}
	w->show();

	return app.exec();
}
