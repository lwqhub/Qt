#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "mouse.h"
#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QLabel *showNum1;
    QLabel *showNum2;
    QLabel *showNum3;
    QLabel *showNum4;
    QLabel *showNum5;
    QLabel *showNum6;
    QPushButton *startButton;
    QPushButton *stopButton;
    QPushButton *resetButton;
    Mouse *myThread;
signals:
    void ResetSignal();
public slots:
    void ClearSlot();
    void StartSlot();
    void StopSlot();
    void UpdateSlot(long,long,long,long,long,long);
};

#endif // MAINWINDOW_H
