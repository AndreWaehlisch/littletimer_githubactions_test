#include <string> // std::stod
#include <stdexcept> // std::exception, std::invalid_argument
#include <climits> // INT_MAX

#include <QMessageBox>
#include <QTimer>
#include "ui_mainwindow.h"

#include "simpletimer.h"

SimpleTimer::SimpleTimer(const Ui::MainWindow& ui)
{
    running = false;
    theLineEdit = ui.lineEdit;
    thePushButton = ui.pushButton;
    theComboBox = ui.comboBox;
}

void SimpleTimer::timerFired()
{
    running = false;
    theLineEdit->setDisabled(false);
    theComboBox->setDisabled(false);
    thePushButton->setText("Start");
    QMessageBox::information(thePushButton->parentWidget(), "Info", "TIMER FIRED!");
}

void SimpleTimer::startStopTimer()
{
    if (running)
    {
        running = false;
        thePushButton->setText("Start");
        theLineEdit->setDisabled(false);
        theComboBox->setDisabled(false);
    }
    else
    {
        const std::string str = theLineEdit->text().toStdString(); // holds the user input
        double input = 0.; // for conversion of user input
        size_t idx = 0; // is set by "std::stod" to position of the next character in "str" after the numerical value
        unsigned long factor; // factor to convert input value to ms

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

        QTimer::singleShot(input * factor, this, SLOT(timerFired())); // convert input to msec and start a single shot timer
        running = true;
        thePushButton->setText("Stop");
        theLineEdit->setDisabled(true);
        theComboBox->setDisabled(true);
    }
}
