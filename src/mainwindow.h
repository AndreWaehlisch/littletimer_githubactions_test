#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <simpletimer.h>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = Q_NULLPTR);
        ~MainWindow();
        static QVector<MainWindow *> windowPointer_vector; // list of "MainWindow" pointers

    private slots:
        void on_pushButton_clicked() const; // Start/Stop button
        void on_lineEdit_textEdited(const QString &arg1) const; // input edit field
        void on_pushButton_2_clicked() const; // "+" button
        void closeEvent(QCloseEvent *event); // called when the window is closed (the "X" in the top right is clicked)

    private:
        Ui::MainWindow *ui;
        SimpleTimer *myTimer;
        int ID; // identification of "this" (type "MainWindow") in the QVector "windowPointer_vector"
};

#endif // MAINWINDOW_H
