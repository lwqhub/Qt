#include "joystick.h"
#include "qdebug.h"

#define SAFE_RELEASE(p)     { if(p) { (p)->Release(); (p) = NULL; } }

joystick::joystick(QObject *parent)
	: QObject(parent)
{
	this->id = 0;
	device_counter = 0;

	di = NULL;
	joyStick = NULL;
}

joystick::~joystick()
{
	close();
}

HRESULT joystick::deviceName(char* name)
{
	HRESULT hr;
	DIDEVICEINSTANCE device;

	ZeroMemory(&device, sizeof(device)); //用0来填充一块内存区域

	device.dwSize = sizeof(device);

	if (!di || !joyStick)
		return E_INVALIDARG;
	if (FAILED(hr = joyStick->GetDeviceInfo(&device)))
		return hr;

	//strncpy(name, device.tszProductName, MAX_PATH); //tszProductName: Friendly name for the product.

	char*  pElementText;
	int    iTextLen;
	// wide char to multi char
	iTextLen = WideCharToMultiByte(CP_UTF8,
		0,
		device.tszProductName,
		-1,
		NULL,
		0,
		NULL,
		NULL);
	pElementText = new char[iTextLen + 1];
	memset((void*)pElementText, 0, sizeof(char) * (iTextLen + 1));
	::WideCharToMultiByte(CP_UTF8,
		0,
		device.tszProductName,
		-1,
		pElementText,
		iTextLen,
		NULL,
		NULL);

	strncpy(name, pElementText, MAX_PATH);
	//strncpy(name, device.tszInstanceName, MAX_PATH);
	//memcpy(name, device.tszInstanceName, MAX_PATH);
	return hr;

}

HRESULT	joystick::open()
{
	HRESULT hr;

	//Initialization: Before you can access the joystick, you must first initialize the DirectInput library
	//This function call will initialize the interface to Direct Input. Once you have a Direct Input object you can initialize other input devices.
	if (FAILED(hr = DirectInput8Create(GetModuleHandle(NULL),      // Create a DirectInput device
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(VOID**)&di, NULL)))
		return hr;

	/*
	每个安装的设备都有一个系统分配的全局惟一标识符（global unique identification, GUID）数字。要使用一个设备，首先必须知道它的GUID。对于连接到系统上的鼠标和键盘，
	得到他们的GUID非常容易，DirectInput分别为鼠标和键盘的GUID定义成GUID_SysKeyboard和GUID_SysMouse。要使用GUID_SysKeyboard或 GUID_SysMouse，必须在所有其
	他的预处理程序指令前定义INITGUID，或者将DXGuid.lib库链接到项目中。至于其他设备，必须枚举出这些设备，才能得到需要的那些设备的GUID。枚举就是遍历一个含有数据项的列表
	的过程，数据项就是诸如游戏杆之类的输入设备。假设有5个游戏杆连接到了系统上，那么在枚举的过程中，DirectInput就会传递各个游戏杆的相关信息，而且每次只传递一个游戏杆的信息
	直到所有的游戏杆都已经被列出来或者列举被强行终止。
	*/

	// Selecting a Joystick 
	// DirectInput uses the EnumDevices() method to iterate through all of the available input devices on the system.
	if (FAILED(hr = di->EnumDevices(DI8DEVCLASS_GAMECTRL, //Device type filter.To restrict the enumeration to a particular type of device
		::enumCallback, //Address of a callback function to be called once for each device enumerated.
		(LPVOID)this,//Application-defined 32-bit value to be passed to the enumeration callback each time it is called.
		DIEDFL_ATTACHEDONLY))) //Flag value that specifies the scope of the enumeration(DIEDFL_ATTACHEDONLY:Only attached and installed devices.)
		return hr;


	// Make sure we got a joystick
	if (joyStick == NULL)
		//printf("Joystick not found.\n");
		return S_OK;


	// 每种设备都有一种用于读取数据的特定数据格式，需要考虑的东西也很多，包括键、鼠标按键、轴等。
	// 因此要使程序从设备读取数据，首先必须告诉 DirectInput读取这种数据所采用的格式
	// Set the data format to "simple joystick" - a predefined data format 
	// A data format specifies which controls on a device we are interested in, and how they should be reported. This tells DInput that we will be
	// passing a DIJOYSTATE2 structure to IDirectInputDevice::GetDeviceState().

	if (FAILED(hr = joyStick->SetDataFormat(&c_dfDIJoystick2)))
		//参数LPCDIDATAFORMAT lpdf: Address of a structure that describes the format of the data that the DirectInputDevice should return.
		//An application can define its own DIDATAFORMAT structure or use one of the following predefined global variables:
		return hr;


	// Enumerate the Joystick objects. The callback function enabled user
	// interface elements for objects that are found, and sets the min/max
	// values property for discovered axes.
	if (FAILED(hr = joyStick->EnumObjects(::enumObjectsCallback, (LPVOID)this, DIDFT_ALL)))
		return hr;

	// 每个程序都会使用许多输入设备。几乎每个程序都会使用键盘和鼠标，同时某些程序还会使用游戏杆。在处理的时候，必须同其他可能仍在运行的应用程序共享对这些设备的访问
	// 或者应用程序独占对设备的所有访问，这样除非该应用程序结束了对设备的访问，否则就不允许其他应用程序控制这些设备。
	// Set the cooperative level to let DInput know how this device should interact with the system and with other DInput applications.
	// DISCL_FOREGROUND: If foreground access is granted, the device is automatically unacquired when the associated window moves to the background.
	// DISCL_BACKGROUND: If background access is granted, the device can be acquired at any time, even when the associated window is not the active window.
	// DISCL_EXCLUSIVE:  If exclusive access is granted, no other instance of the device can obtain exclusive access to the device while it is acquired.
	if (FAILED(hr = joyStick->SetCooperativeLevel(NULL, DISCL_EXCLUSIVE | DISCL_FOREGROUND)))
		//if (FAILED(hr = joyStick->SetCooperativeLevel(NULL, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE)))
		return hr;

	return S_OK;
}

HRESULT joystick::getCapability() //Describes a DirectInput device's capabilities.
{
	HRESULT hr;
	//dwSize: Size of this structure, in bytes. This member must be initialized by the application before a call to the IDirectInputDevice8::GetCapabilities method.
	capabilities.dwSize = sizeof(DIDEVCAPS);
	if (FAILED(hr = joyStick->GetCapabilities(&capabilities))) //获取设备信息,如设备类型,轴数,按键数等
		return hr;

	//DWORD dwTpye = capabilities.dwDevType;
	//int nAxes = capabilities.dwAxes;       // Number of axes available on the device. 
	//int nButtons = capabilities.dwButtons; // Number of buttons available on the device. 
	//int nPOVs =	capabilities.dwPOVs;       // Number of point-of-view controllers available on the device. 
}

HRESULT	joystick::close()
{
	if (joyStick)
		joyStick->Unacquire(); //Releases access to the device

	SAFE_RELEASE(joyStick);
	SAFE_RELEASE(di);

	return S_OK;
}


HRESULT	joystick::updateInputState(DIJOYSTATE2 *js)
{
	HRESULT hr;

	if (joyStick == NULL)
		return S_OK;

	// Poll the device to read the current state
	hr = joyStick->Poll(); //Retrieves data from polled objects on a DirectInput device.
	if (FAILED(hr))
	{
		//DirectInput is telling us that the input stream has been interrupted. We aren't tracking any state between
		//polls, so we don't have any special reset that needs to be done.  We just re-acquire and try again.
		hr = joyStick->Acquire(); //Obtains access to the input device. 
		while (hr == DIERR_INPUTLOST)
			hr = joyStick->Acquire();


		//Before a device can be acquired, a data format must be set by using the IDirectInputDevice8::SetDataFormat method
		//or IDirectInputDevice8::SetActionMap method. If the data format has not been set, IDirectInputDevice8 Interface returns DIERR_INVALIDPARAM.
		if ((hr == DIERR_INVALIDPARAM) || (hr == DIERR_NOTINITIALIZED))
			return E_FAIL;

		// If another application has control of this device, return success.
		// We'll just have to wait our turn to use the joystick.
		if (hr == DIERR_OTHERAPPHASPRIO)
			return S_OK;
	}


	// Devices must be acquired before calling the IDirectInputDevice8::GetDeviceState 
	// or IDirectInputDevice8::GetDeviceData methods for that device.
	//参数1: Size of the buffer in the lpvData parameter, in bytes.
	//参数2: Address of a structure that receives the current state of the device. The format of the data is 
	//established by a prior call to the IDirectInputDevice8::SetDataFormat method.
	if (FAILED(hr = joyStick->GetDeviceState(sizeof(DIJOYSTATE2), js)))    // Get the input's device state 
		return hr;

	return S_OK;
}

BOOL CALLBACK joystick::enumCallback(const DIDEVICEINSTANCE* instance, VOID* context) //在系统上每找到一个匹配的设备时，就会调用该函数。
{
	//函数参数1是一个指向DIDEVICEINSTANCE结构体的指针，此结构体包含了此次调用时当前枚举设备上的信息
	//DIDEVICEINSTANCE Structure 参考下面网址
	//https://msdn.microsoft.com/en-us/library/windows/desktop/microsoft.directx_sdk.reference.dideviceinstance(v=vs.85).aspx

	// If this is the requested device ID ...
	if (device_counter == this->id)
	{
		// Obtain an interface to the enumerated joystick.Stop the enumeration if the requested device was created successfully.
		if (SUCCEEDED(di->CreateDevice(instance->guidInstance, //guidInstance:Unique identifier for the instance of the device. 
			&joyStick, //Address of a variable to receive the IDirectInputDevice8 Interface interface pointer if successful
			NULL))) {
			return DIENUM_STOP;
		}

	}

	// Otherwise, increment the device counter and continue with the device enumeration.
	device_counter++;
	return DIENUM_CONTINUE;
}

BOOL CALLBACK enumCallback(const DIDEVICEINSTANCE* instance, VOID* context)
{
	if (context != NULL)
		return ((joystick *)context)->enumCallback(instance, context);
	else
		return DIENUM_STOP;
	//Returns DIENUM_CONTINUE to continue the enumeration or DIENUM_STOP to stop the enumeration.
}

BOOL CALLBACK enumObjectsCallback(const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* context)
{
	if (context != NULL)
		return ((joystick *)context)->enumObjectsCallback(pdidoi, context);
	else
		return DIENUM_STOP;
}


BOOL CALLBACK joystick::enumObjectsCallback(const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* context)
{
	// For axes that are returned, set the DIPROP_RANGE property for the enumerated axis in order to scale min/max values.
	if (pdidoi->guidType == GUID_XAxis || pdidoi->guidType == GUID_YAxis)  //guidType: Unique identifier that indicates the object type
	{
		DIPROPRANGE diprg;
		diprg.diph.dwSize = sizeof(DIPROPRANGE);//Size of the enclosing structure. This member must be initialized before the structure is used. 
		diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);//Size of the DIPROPHEADER structure. 
		diprg.diph.dwHow = DIPH_BYID; //Value that specifies how the dwObj member should be interpreted
		diprg.diph.dwObj = pdidoi->dwType; // Specify the enumerated axis
		diprg.lMin = 0;     //Lower limit of the range.
		diprg.lMax = 16383; //Upper limit of the range

							// Set the range for the axis
		if (FAILED(joyStick->SetProperty(DIPROP_RANGE, &diprg.diph))) //Sets properties that define the device behavior
			return DIENUM_STOP;

	}


	//设置Z轴旋转时返回值范围: 0-255
	if (pdidoi->guidType == GUID_RzAxis || pdidoi->guidType == GUID_Slider)
	{
		DIPROPRANGE diprg;
		diprg.diph.dwSize = sizeof(DIPROPRANGE);//Size of the enclosing structure. This member must be initialized before the structure is used. 
		diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);//Size of the DIPROPHEADER structure. 
		diprg.diph.dwHow = DIPH_BYID; //Value that specifies how the dwObj member should be interpreted
		diprg.diph.dwObj = pdidoi->dwType; // Specify the enumerated axis
		diprg.lMin = 0;     //Lower limit of the range.
		diprg.lMax = 255;   //Upper limit of the range

							// Set the range for the axis
		if (FAILED(joyStick->SetProperty(DIPROP_RANGE, &diprg.diph))) //Sets properties that define the device behavior
			return DIENUM_STOP;

	}

	//设置滑块滑动时返回值范围: 0-100
	if (pdidoi->guidType == GUID_Slider)
	{
		DIPROPRANGE diprg;
		diprg.diph.dwSize = sizeof(DIPROPRANGE);//Size of the enclosing structure. This member must be initialized before the structure is used. 
		diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);//Size of the DIPROPHEADER structure. 
		diprg.diph.dwHow = DIPH_BYID; //Value that specifies how the dwObj member should be interpreted
		diprg.diph.dwObj = pdidoi->dwType; // Specify the enumerated axis
		diprg.lMin = 0;     //Lower limit of the range.
		diprg.lMax = 100;   //Upper limit of the range

							// Set the range for the axis
		if (FAILED(joyStick->SetProperty(DIPROP_RANGE, &diprg.diph))) //Sets properties that define the device behavior
			return DIENUM_STOP;

	}

	return DIENUM_CONTINUE;
}

unsigned int joystick::deviceCount()
{
	unsigned int counter = 0;
	LPDIRECTINPUT8 di = NULL;
	HRESULT hr;

	if (SUCCEEDED(hr = DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&di, NULL)))
		di->EnumDevices(DI8DEVCLASS_GAMECTRL, ::countCallback, &counter, DIEDFL_ATTACHEDONLY);

	return counter;
}


BOOL CALLBACK countCallback(const DIDEVICEINSTANCE* instance, VOID* counter)
{
	if (counter != NULL)
	{
		unsigned int *tmpCounter = (unsigned int *)counter;
		(*tmpCounter)++;
		counter = tmpCounter;
	}

	return DIENUM_CONTINUE;
}

