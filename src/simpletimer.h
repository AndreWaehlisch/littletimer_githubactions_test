#ifndef SIMPLETIMER_H
#define SIMPLETIMER_H

#include "ui_mainwindow.h"

enum class conversion_factor { ms, sec, min, h };

class SimpleTimer : public QObject
{
    Q_OBJECT

    private:
        QPushButton *thePushButton;
        QLineEdit *theLineEdit;
        QComboBox *theComboBox;

    public:
        bool running; // is the timer currently running?

        SimpleTimer(const Ui::MainWindow& ui); // constructor, nee
        void startStopTimer(); // start or stop the timer, depending if "running" is true or false

    public slots:
        void timerFired();
};

#endif // SIMPLETIMER_H
