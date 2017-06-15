#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QSystemTrayIcon>
#include <QMenu>

#include "ui_mainwindow.h"

namespace Ui {
    class MainWindow;
}

class SimpleTimer;

class MainWindow : public QMainWindow {
        Q_OBJECT

    public:
        explicit MainWindow(const QString &windowTitle = "Little Timer", QWidget *parent = Q_NULLPTR);
        ~MainWindow();
        QSystemTrayIcon *myTray;

    private slots:
        void tray_clicked(QSystemTrayIcon::ActivationReason) const;
        void pushButton_clicked() const; // Start/Stop button
        void lineEdit_textEdited(const QString &arg1) const; // input edit field
        void pushButton_2_clicked() const; // "+" button
        void closeEvent(QCloseEvent *);
        void hideEvent(QHideEvent *);
        void showEvent(QShowEvent *);

    private:
        Ui::MainWindow *ui;
        SimpleTimer *myTimer;
        QMenu *myTrayMenu;
        bool isClosed = false;
};

#endif // MAINWINDOW_H
