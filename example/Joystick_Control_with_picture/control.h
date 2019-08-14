#ifndef CONTROL_H
#define CONTROL_H

#include <QMainWindow>
#include "joystick_thread.h"

namespace Ui {
class Control;
}

class Control : public QMainWindow
{
    Q_OBJECT

public:
    explicit Control(QWidget *parent = 0);
    ~Control();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Control *ui;
    Joystick_Thread *mythread;
    BYTE state[128][2] = {0};

    void Show_or_Hide_the_Button(DIJOYSTATE2 js1);

public slots:
    void UpdateSlot(DIJOYSTATE2 js1);
};

#endif // CONTROL_H
