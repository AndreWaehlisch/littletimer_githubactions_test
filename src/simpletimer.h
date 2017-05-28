#ifndef SIMPLETIMER_H
#define SIMPLETIMER_H

#include "ui_mainwindow.h"

#include <QTimer>

enum class conversion_factor { ms, sec, min, h };

class SimpleTimer : public QObject {
        Q_OBJECT

    private:
        mutable QString myFactorString; // factor string for progressbar label ("min"/"sec")
        mutable QString myRemainingTimeString; // remaining time string for progressbar label ("6" in "6min")
        QMainWindow *theMainWindow;
        QPushButton *thePushButton;
        QLineEdit *theLineEdit;
        QComboBox *theComboBox;
        QProgressBar *theProgressBar;
        QTimer myTimer; // when user requests 6min countdown, this timer deals with that
        QTimer myProgressBarUpdateTimer; // this timer fires every second and updates the progress bar
        void startStuff(); // does stuff when timer is started (e.g. disable button)

    public:
        bool running; // if the timer is currently running
        SimpleTimer(const Ui::MainWindow * const ui, QMainWindow * const mainwindow); // constructor
        void startStopTimer(); // start or stop the timer, depending if "running" is true or false

    public slots:
        void stopStuff(); // does stuff when timer is stopped  (e.g. enable button)
        void timerFired() const; // does stuff to inform user that timer has ended
        void updateProgressBar() const; // update theProgressBar continuously
};

#endif // SIMPLETIMER_H
