#pragma once
#ifndef JOYSTICK_H
#define JOYSTICK_H
#define DIRECTINPUT_VERSION 0x800

#include <QObject>

#include <Windows.h>
#include <qstring.h>
#include <dinput.h>

#pragma comment (lib,"dinput8.lib")
#pragma comment (lib,"dxguid.lib")
#include <QObject>

class joystick : public QObject
{
	Q_OBJECT

public:
	joystick(QObject *parent = Q_NULLPTR);
    ~joystick();

	HRESULT deviceName(char* name); //获取设备名
	HRESULT getCapability();        //获取设备的类型及轴数、按钮数量等信息

	HRESULT open();
	HRESULT close();

	HRESULT updateInputState(DIJOYSTATE2 *js);  //Update the device state.

	BOOL CALLBACK enumCallback(const DIDEVICEINSTANCE* instance, VOID* context);
	BOOL CALLBACK enumObjectsCallback(const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* context);

	// Device Querying
	static unsigned int deviceCount();

private:
	unsigned int id;
	unsigned int device_counter;
	DIDEVCAPS capabilities;

	LPDIRECTINPUT8 di;
	LPDIRECTINPUTDEVICE8 joyStick;

};
BOOL CALLBACK enumCallback(const DIDEVICEINSTANCE* instance, VOID* context);
BOOL CALLBACK countCallback(const DIDEVICEINSTANCE* instance, VOID* counter);
BOOL CALLBACK enumObjectsCallback(const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* context);

#endif // JOYSTICK_H
