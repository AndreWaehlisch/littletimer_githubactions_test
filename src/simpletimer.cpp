#include <limits> // std::numeric_limits
#include <QMessageBox>

#include "simpletimer.h"

SimpleTimer::SimpleTimer(const Ui::MainWindow& ui) : myTimer(this), myProgressBarUpdateTimer(this)
{
    running = false;

    myTimer.setSingleShot(true); // timer only fires once
    connect(&myTimer, SIGNAL(timeout()), this, SLOT(stopStuff())); // call our "stopStuff" func when timer expires
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

void SimpleTimer::updateProgressBar() const
{
    // progress bar value
    const double percent = 100.0*myTimer.remainingTime()/myTimer.interval();
    theProgressBar->setValue((int) nearbyint(percent));

    // label text
    if( myTimer.remainingTime() > 60000 ) // >1min
    {
        myFactorString = "min";

        if( myTimer.remainingTime() > 60000*5 ) // >5min
        {
            myRemainingTimeString.setNum(ceil(myTimer.remainingTime()/60000.), 'f', 0); // for "big minutes" we just use the minute (always round up to full minutes)
        }
        else // <=5min and >1min
            myRemainingTimeString.setNum(myTimer.remainingTime()/60000., 'f', 1); // for "small minutes" we use one number after the decimal delimiter (rounds to next 0.1 minute). "showpoint" forces the decimal point.
    }
    else
    {
        myFactorString = "sec";
        myRemainingTimeString.setNum(myTimer.remainingTime()/1000., 'f', 0); // round to full seconds
    }

    theProgressBar->setFormat(myRemainingTimeString + myFactorString);
}

void SimpleTimer::startStuff()
{
    running = true;

    thePushButton->setText("Stop");
    theLineEdit->setDisabled(true);
    theComboBox->setDisabled(true);
    theProgressBar->setEnabled(true);

    myTimer.start();
    myProgressBarUpdateTimer.start();
    updateProgressBar(); // ProgressBarUpdateTimer does not run until 1s after we start our stuff, so do a manual update here
}

void SimpleTimer::stopStuff()
{
    myTimer.stop();
    myProgressBarUpdateTimer.stop();

    running = false;

    thePushButton->setText("Start");
    theLineEdit->setDisabled(false);
    theComboBox->setDisabled(false);
    theProgressBar->setEnabled(false);
    theProgressBar->setValue(0);
    theProgressBar->setFormat("");
}

void SimpleTimer::timerFired() const
{
    QMessageBox msg(QMessageBox::Warning, tr("Info"), tr("Timer fired!"), QMessageBox::Ok, thePushButton);
    msg.setWindowModality(Qt::WindowModal);
    msg.exec();
}

void SimpleTimer::startStopTimer()
{
    // If running: Stop timer. Else: Start timer.
    if (running)
        stopStuff();
    else
    {
        double input; // for conversion of user input
        unsigned long factor; // factor to convert input value to ms
        const QString inputString = theLineEdit->text(); // holds the user input

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
                QMessageBox::warning(thePushButton->parentWidget(), tr("Tool outdated"), tr("The selected conversion factor is unknown!"));
                return;
        }

        // Convert user input
        bool conversionOkay;
        input = inputString.toDouble(&conversionOkay); // try to convert QString to double

        // Test if conversion was okay (see http://doc.qt.io/qt-5/qstring.html#toDouble) [note: if not ok, then input=0]. QTimer uses int (msec), so make sure we are in the limit of that, also check for negative numbers.
        if(!conversionOkay || input*factor > std::numeric_limits<int>::max() || input < 0.)
        {
            QMessageBox::warning(thePushButton->parentWidget(), tr("Info"), tr("Invalid input! Must be a positive number, which can't be too big (max 596h)."));
            return;
        }

        // if input is zero we don't have to do anything
        if(input == 0.)
            return;

        myTimer.setInterval(input*factor); // convert input to msec and start the (single shot) timer
        startStuff();
    }
}
