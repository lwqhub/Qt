#ifndef JOYSTICK_THREAD_H
#define JOYSTICK_THREAD_H

#include <QMainWindow>
#include <QThread>
#include <QMetaType>
#include "joystick.h"

class Joystick_Thread : public QThread
{
     Q_OBJECT
public:
    Joystick_Thread(QObject *parent=0);
    ~Joystick_Thread();

private:
    joystick joyStick;
    DIJOYSTATE2 js1;

protected:
    void run();

signals:
    void UpdateSignal(DIJOYSTATE2 js1);
};

#endif // JOYSTICK_THREAD_H
