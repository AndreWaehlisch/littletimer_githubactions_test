#include "mainwindow.h"
#include "simpletimer.h"

#include <QInputDialog>
#include <QScreen>

// initializing static members
unsigned long MainWindow::currentNumWindows = 0; // current active number of windows, if this drops to 0 we close the application
unsigned long MainWindow::theWindowID = 0; // ascending ID of each MainWindow, starting with 0
QIcon MainWindow::theIcon; // must construct QIcon object after QMainApplication, so do just that in main.cpp

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
    myTray->hide(); // hide tray icon on creation
    myTray->setContextMenu(myTrayMenu);

    connect(myTray, &QSystemTrayIcon::activated, this, &MainWindow::tray_clicked); // show menu on (right) click
    connect(ui->pushButton, &QPushButton::clicked, myTimer, &SimpleTimer::startStopTimer);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &MainWindow::pushButton_2_clicked);
    connect(ui->lineEdit, &QLineEdit::textChanged, this, &MainWindow::lineEdit_textEdited);
    connect(ui->lineEdit, &QLineEdit::returnPressed, myTimer, &SimpleTimer::startStopTimer);

    this->setWindowTitle(windowTitle);
    this->move(QGuiApplication::primaryScreen()->geometry().center() - this->geometry().center()); // position window on screen center
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

void MainWindow::lineEdit_textEdited(const QString &arg1) const {
    ui->pushButton->setEnabled(arg1.length() > 0); // disable pushbutton if user input is empty
    ui->comboBox->setVisible(!arg1.contains(":"));
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
    if(!isClosed && QSystemTrayIcon::isSystemTrayAvailable()) {
        myTray->show();
        this->hide();
    }
}

void MainWindow::showEvent(QShowEvent *) {
    myTray->hide();
    this->show();
    this->activateWindow();
    this->raise();
}

void MainWindow::pushButton_2_clicked() const {
    QString tempWindowTitle = tr("Little Timer ") + QString::number(theWindowID);

    if(QGuiApplication::keyboardModifiers() & Qt::ShiftModifier) {
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
