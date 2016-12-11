#ifndef SIMPLETIMER_H
#define SIMPLETIMER_H

#include "ui_mainwindow.h"
#include <QTimer>

enum class conversion_factor { ms, sec, min, h };

class SimpleTimer : public QObject
{
    Q_OBJECT

    private:
        QPushButton *thePushButton;
        QLineEdit *theLineEdit;
        QComboBox *theComboBox;
        QProgressBar *theProgressBar;
        QTimer myTimer;
        QTimer myProgressBarUpdateTimer;
        void startStuff(); // does stuff when timer is started (e.g. disable button)

    public:
        bool running; // is the timer currently running?
        SimpleTimer(const Ui::MainWindow& ui); // constructor
        void startStopTimer(); // start or stop the timer, depending if "running" is true or false

    public slots:
        void stopStuff(); // does stuff when timer is stopped  (e.g. enable button)
        void timerFired() const; // does stuff to inform user that timer has ended
        void updateProgressBar() const; // update theProgressBar continuously
};

#endif // SIMPLETIMER_H
