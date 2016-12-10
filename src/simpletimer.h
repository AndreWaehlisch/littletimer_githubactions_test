#ifndef SIMPLETIMER_H
#define SIMPLETIMER_H

#include "ui_mainwindow.h"
#include <QSystemTrayIcon>

enum class conversion_factor { ms, sec, min, h };

class SimpleTimer : public QObject
{
    Q_OBJECT

    private:
        QPushButton *thePushButton;
        QLineEdit *theLineEdit;
        QComboBox *theComboBox;
        QSystemTrayIcon *theSystemTrayIcon;
        QIcon *theSystemTrayIcon_Icon;
        QTimer *theTimer;

    public:
        bool running; // is the timer currently running?

        SimpleTimer(const Ui::MainWindow& ui); // constructor
        ~SimpleTimer(); // destructor
        void startStopTimer(); // start or stop the timer, depending if "running" is true or false
        void startStuff(); // does stuff when timer is started (e.g. disable button)
        void stopStuff(); // does stuff when timer is stopped  (e.g. enable button)

    public slots:
        void timerFired();
};

#endif // SIMPLETIMER_H
