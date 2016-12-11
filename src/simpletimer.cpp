#include <string> // std::stod, std::to_string
#include <stdexcept> // std::exception, std::invalid_argument
#include <limits> // std::numeric_limits
#include <sstream> // std::ostringstream
#include <iomanip> // std::setprecision, std::showpoint

#include "ui_mainwindow.h"
#include <QMessageBox>

#include "simpletimer.h"

SimpleTimer::SimpleTimer(const Ui::MainWindow& ui) : mySystemTray_Icon(":/bell.ico"), mySystemTray(mySystemTray_Icon), myTimer(this), myProgressBarUpdateTimer(this)
{
    running = false;

    myTimer.setSingleShot(true); // timer only fires once
    connect(&myTimer, SIGNAL(timeout()), this, SLOT(timerFired())); // call our "timerFired" func when timer expires

    myProgressBarUpdateTimer.setSingleShot(false); // fire repeatedly
    myProgressBarUpdateTimer.setInterval(1000); // fire once per second
    connect(&myProgressBarUpdateTimer, SIGNAL(timeout()), this, SLOT(updateProgressBar()));

    // get some pointers to ui elements
    theLineEdit = ui.lineEdit;
    thePushButton = ui.pushButton;
    theComboBox = ui.comboBox;
    theProgressBar = ui.progressBar;
}

void SimpleTimer::updateProgressBar()
{
    // progress bar value
    const double percent = 100.0*myTimer.remainingTime()/myTimer.interval();
    theProgressBar->setValue((int) nearbyint(percent));

    // label text
    std::string factorString;
    std::ostringstream remainingTimeStringStream;
    remainingTimeStringStream.setf(std::ios::fixed, std::ios::floatfield); // fixed = decimal notation (>not< scientific notation), floatfield makes sure all other flags are unset in the group

    if( myTimer.remainingTime() > 60000 ) // >1min
    {
        factorString = "min";

        if( myTimer.remainingTime() > 60000*5 ) // >5min
            remainingTimeStringStream << std::setprecision(0) << ceil(myTimer.remainingTime()/60000.); // for "big minutes" we just use the minute (round up to full minutes)
        else // <=5min and >1min
            remainingTimeStringStream << std::setprecision(1) << std::showpoint << myTimer.remainingTime()/60000.; // for "small minutes" we use one number after the decimal delimiter (rounds to next 0.1 minute). "showpoint" forces the decimal point.
    }
    else
    {
        factorString = "sec";
        remainingTimeStringStream << std::setprecision(0) << myTimer.remainingTime()/1000.; // round to full seconds
    }

    theProgressBar->setFormat(QString::fromStdString(remainingTimeStringStream.str() + factorString));
}

void SimpleTimer::startStuff()
{
    running = true;
    thePushButton->setText("Stop");
    theLineEdit->setDisabled(true);
    theComboBox->setDisabled(true);
    mySystemTray.show();
    theProgressBar->setEnabled(true);
    myTimer.start();
    myProgressBarUpdateTimer.start();
    updateProgressBar();
}

void SimpleTimer::stopStuff()
{
    myTimer.stop();
    myProgressBarUpdateTimer.stop();
    running = false;
    thePushButton->setText("Start");
    theLineEdit->setDisabled(false);
    theComboBox->setDisabled(false);
    mySystemTray.hide();
    theProgressBar->setEnabled(false);
    theProgressBar->setValue(0);
    theProgressBar->setFormat("");
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
        stopStuff();
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
            if(input * factor > std::numeric_limits<int>::max() || input < 0.)
            {
                throw std::out_of_range("out of range ");
            }
        }
        catch(const std::exception& error)
        {
            QMessageBox::information(thePushButton->parentWidget(), "Info", "Invalid input! Must be a positive number, which can't be too big (max 596h).");
            return;
        }

        myTimer.setInterval(input*factor); // convert input to msec and start the (single shot) timer
        startStuff();
    }
}
