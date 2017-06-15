#include "mainwindow.h"
#include "simpletimer.h"

#include <QDesktopWidget>
#include <QInputDialog>

// initializing static members
long unsigned MainWindow::currentNumWindows = 0; // current active number of windows, if this drops to 0 we close the application
long unsigned MainWindow::theWindowID = 0; // ascending ID of each MainWindow, starting with 0
const QIcon MainWindow::theIcon = QIcon(":/hourglass.ico");

MainWindow::MainWindow(const QString &windowTitle, QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    theWindowID++;
    currentNumWindows++;

    ui->setupUi(this);  // setup user interface from "mainwindow.ui" file (corresponding "ui_mainwindow.h" file is generated by the user interface compiler (uic))
    myTimer = new SimpleTimer(ui, this); // create a "simple timer", which does the timing work

    myTrayMenu = new QMenu();
    myTrayMenu->addAction(windowTitle)->setDisabled(true); // create the first (disabled) action as a label with the default window title
    myTrayMenu->addSeparator();
    myTrayMenu->addAction(tr("Restore timer"), this, &MainWindow::showNormal);
    myTrayMenu->addAction(tr("Create new timer"), this, &MainWindow::pushButton_2_clicked);
    myTrayMenu->addAction(tr("Close timer"), this, &MainWindow::close);

    myTray = new QSystemTrayIcon(theIcon); // create a QSystemTrayIcon, which will be shown when the window is minimized
    myTray->setContextMenu(myTrayMenu);

    connect(myTray, &QSystemTrayIcon::activated, this, &MainWindow::tray_clicked); // show menu on (right) click
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::pushButton_clicked);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &MainWindow::pushButton_2_clicked);
    connect(ui->lineEdit, &QLineEdit::textChanged, this, &MainWindow::lineEdit_textEdited);

    this->setWindowTitle(windowTitle);
    this->setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, this->size(), qApp->desktop()->availableGeometry())); // position window on screen center (https://wiki.qt.io/How_to_Center_a_Window_on_the_Screen)
    this->setWindowIcon(theIcon); // main icon displayed on top left of main window
    this->setAttribute(Qt::WA_DeleteOnClose, true); // make sure windows get deleted when they are closed
}

MainWindow::~MainWindow() {
    delete myTray;
    delete myTrayMenu;
    delete myTimer;
    delete ui;
}

void MainWindow::tray_clicked(QSystemTrayIcon::ActivationReason reason) {
    if(reason == QSystemTrayIcon::Context) {
        myTray->show(); // show tray menu on right-click
    } else if(reason == QSystemTrayIcon::DoubleClick) {
        showNormal(); // restore window on double-(left)-click
    }
}

void MainWindow::pushButton_clicked() const {
    myTimer->startStopTimer();
}

void MainWindow::lineEdit_textEdited(const QString &arg1) const {
    ui->pushButton->setEnabled(arg1.length() > 0); // disable pushbutton if user input is empty
}

void MainWindow::closeEvent(QCloseEvent *) {
    isClosed = true; // detect when window is being closed (and not just hidden)
    myTray->hide(); // force the tray icon to close when window is closed

    currentNumWindows--;

    if(!currentNumWindows) {
        QApplication::quit();
    }
}

void MainWindow::hideEvent(QHideEvent *) {
    if(!isClosed) {
        myTray->show();
        this->hide();
    }
}

void MainWindow::showEvent(QShowEvent *) {
    myTray->hide();
    this->show();
}

void MainWindow::pushButton_2_clicked() const {
    QString tempWindowTitle = tr("Little Timer ") + QString::number(theWindowID);

    if(QGuiApplication::keyboardModifiers() && Qt::ShiftModifier) {
        QInputDialog dialog;
        dialog.setWindowTitle(tr("Custom Name"));
        dialog.setLabelText(tr("Set the name of the new timer:"));
        dialog.setTextValue(tempWindowTitle);
        dialog.setWindowIcon(theIcon);

        if(dialog.exec() == QDialog::Accepted) {
            tempWindowTitle = dialog.textValue();
        } else {
            return;
        }
    }

    MainWindow *w = new MainWindow(tempWindowTitle); // create a new timer window
    w->show();
}
