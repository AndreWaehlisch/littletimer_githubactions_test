#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <simpletimer.h>

#include <QSystemTrayIcon>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
        Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = Q_NULLPTR);
        ~MainWindow();

    private slots:
        void on_pushButton_clicked() const; // Start/Stop button
        void on_lineEdit_textEdited(const QString &arg1) const; // input edit field
        void on_pushButton_2_clicked() const; // "+" button
        void hideEvent(QHideEvent *event);
        void showEvent(QShowEvent *event);

    private:
        Ui::MainWindow *ui;
        SimpleTimer *myTimer;
        QMenu *myTrayMenu;
        QSystemTrayIcon *myTray;
};

#endif // MAINWINDOW_H
