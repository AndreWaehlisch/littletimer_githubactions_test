#include <string> // std::stod
#include <stdexcept> // std::exception, std::invalid_argument
#include <climits> // INT_MAX

#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QTimer>
#include <QSystemTrayIcon>

#include "simpletimer.h"

SimpleTimer::SimpleTimer(const Ui::MainWindow& ui)
{
    running = false;

    theLineEdit = ui.lineEdit;
    thePushButton = ui.pushButton;
    theComboBox = ui.comboBox;

    theSystemTrayIcon_Icon = new QIcon(":/bell.ico");
    theSystemTrayIcon = new QSystemTrayIcon(*theSystemTrayIcon_Icon);

    theTimer = new QTimer(this);
    theTimer->setSingleShot(true); // only fire once
    connect(theTimer, SIGNAL(timeout()), this, SLOT(timerFired())); // call our "timerFired" func when timer expires
}

SimpleTimer::~SimpleTimer()
{
    delete(theSystemTrayIcon);
    delete(theSystemTrayIcon_Icon);
    delete(theTimer);
}

void SimpleTimer::startStuff()
{
    running = true;
    thePushButton->setText("Stop");
    theLineEdit->setDisabled(true);
    theComboBox->setDisabled(true);
    theSystemTrayIcon->show();
    theTimer->start();
}

void SimpleTimer::stopStuff()
{
    theTimer->stop();
    running = false;
    thePushButton->setText("Start");
    theLineEdit->setDisabled(false);
    theComboBox->setDisabled(false);
    theSystemTrayIcon->hide();
}

void SimpleTimer::timerFired()
{
    stopStuff();
    QMessageBox::information(thePushButton->parentWidget(), "Info", "TIMER FIRED!");
}

void SimpleTimer::startStopTimer()
{
    // If running: Stop timer. Else: Start timer.
    if (running)
    {
        stopStuff();
    }
    else
    {
        const std::string str = theLineEdit->text().toStdString(); // holds the user input
        double input = 0.; // for conversion of user input
        size_t idx = 0; // is set by "std::stod" to position of the next character in "str" after the numerical value
        unsigned long factor; // factor to convert input value to ms

        // Check which conversion factor user has selected
        switch (static_cast<conversion_factor>(theComboBox->currentIndex())) {
            case conversion_factor::ms:
                factor = 1;
                break;
            case conversion_factor::sec:
                factor = 1000;
                break;
            case conversion_factor::min:
                factor = 1000 * 60;
                break;
            case conversion_factor::h:
                factor = 1000 * 60 * 60;
                break;
            default:
                QMessageBox::warning(thePushButton->parentWidget(), "Tool outdated", "The selected conversion factor is unknown!");
                return;
        }

        // Convert user input
        try
        {
            input = std::stod(str, &idx); // try to convert str to double

            // if input is zero we don't have to do anything
            if(input == 0.)
                return;

            // the conversion was okay, but after the numerical value there are some more characters (e.g. "33.0f", or even "11.3E1y"): we don't allow this.
            if(idx < str.length())
            {
                throw std::invalid_argument("idx<str.length");
            }

            // QTimer uses int (msec), so make sure we are in the limit of that, also check for negative numbers
            if(input * factor > INT_MAX || input < 0.)
            {
                throw std::out_of_range("out of range ");
            }
        }
        catch(const std::exception& error)
        {
            QMessageBox::information(thePushButton->parentWidget(), "Info", "Invalid input!");
            return;
        }

        theTimer->setInterval(input*factor); // convert input to msec and start the (single shot) timer
        startStuff();
    }
}
