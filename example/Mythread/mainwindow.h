#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>

class Thread : public QThread
{
    Q_OBJECT
public:
    Thread(QObject *parent=0);
    ~Thread();
private:
    int number1;
    int number2;
    int number3;
    int number4;
    int number5;
    int number6;
protected:
    void run();
signals:
    void UpdateSignal(int num1,int num2,int num3,int num4,int num5,int num6);
public slots:
    void ResetSlot();
};

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
    Thread *myThread;
    int number;
signals:
    void ResetSignal();
public slots:
    void ClearSlot();
    void StartSlot();
    void StopSlot();
    void UpdateSlot(int num1,int num2,int num3,int num4,int num5,int num6);
};

#endif // MAINWINDOW_H
