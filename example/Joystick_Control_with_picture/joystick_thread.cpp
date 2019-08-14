#include "joystick_thread.h"

Joystick_Thread::Joystick_Thread(QObject *parent)
{
    qRegisterMetaType<DIJOYSTATE2>("DIJOYSTATE2");//注册DIJOYSTATE2类型
    joyStick.open();
}

Joystick_Thread::~Joystick_Thread()
{
    joyStick.close();
}

void Joystick_Thread::run()
{
    while(1)
    {
        joyStick.updateInputState(&js1);
        emit UpdateSignal(js1);
        msleep(10);
    }
}
