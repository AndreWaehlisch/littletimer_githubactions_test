#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "simpletimer.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked() const
{
    static SimpleTimer myTimer(*ui);
    myTimer.startStopTimer();
}

void MainWindow::on_lineEdit_textEdited(const QString &arg1) const
{
    ui->pushButton->setEnabled( (arg1.length() > 0) ); // disable pushbutton if user input is empty
}
