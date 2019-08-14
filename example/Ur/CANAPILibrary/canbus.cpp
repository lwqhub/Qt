#include "canbus.h"
#include <QStringList>
#include <QDebug>
CanBus::CanBus(QObject *parent) : QObject(parent)
{
    openFlag = false;


    // init current frame
    VCI_CAN_OBJ currentFrame;
    currentFrame.DataLen = 7; // 数据长度�?7
    currentFrame.Data[0] = 0xaa;
    currentFrame.Data[1] = 0x00;
    currentFrame.Data[2] = 0xb1;
    currentFrame.Data[3] = 0x00;
    currentFrame.Data[4] = 0x00;
    currentFrame.Data[5] = 0x00;
    currentFrame.Data[6] = 0xff;
    currentFrame.ID= currentFrame.Data[1];  //获取帧ID
    currentFrame.SendType = 0;   //正常发�?
    currentFrame.RemoteFlag = 0; //数据�?
    currentFrame.ExternFlag = 0; //标准�?
}

bool CanBus::openCAN(DEVICETYPE deviceType, int deviceIndex, int canIndex)
{
    m_deviceType = deviceType;
    m_deviceIndex = deviceIndex;
    m_canIndex = canIndex;
//    qDebug()<<"m_deviceType"<<m_deviceType<<"m_deviceIndex"<<m_deviceIndex;
	DWORD re = VCI_OpenDevice(m_deviceType,m_deviceIndex, 0);
    if( re== STATUS_OK)  //打开设备
       return true;
    else
        return false;
}

bool CanBus::initCAN()
{
    //默认配置参数
    m_CAN_Initstruct.AccCode=0x00000000;
    m_CAN_Initstruct.AccMask=0xffffffff;
    m_CAN_Initstruct.Filter=0;//0双滤波，1单滤�?
    m_CAN_Initstruct.Mode=0;   //0正常模式�?1只听模式
    m_CAN_Initstruct.Timing0=0x00;
    m_CAN_Initstruct.Timing1=0x1C;

    if(initCAN(m_CAN_Initstruct))
        return true;
    else
        return false;
}

bool CanBus::initCAN(VCI_INIT_CONFIG CAN_Initstruct)
{
    if(VCI_InitCAN(m_deviceType,m_deviceIndex,m_canIndex,&CAN_Initstruct)!=STATUS_OK) //初始化设�?
    {
        VCI_CloseDevice(m_deviceType,m_deviceIndex);
        return false;
    }
    else
    {
        openFlag = true;
        return true;
    }
}

void CanBus::startRecvThread()
{
    pRecvThread = new CanRecvThread();
    pRecvThread->init(m_deviceType,m_deviceIndex,m_canIndex);
    connect(pRecvThread,SIGNAL(recvdata(VCI_CAN_OBJ)),this,SLOT(receiveFromCANThread(VCI_CAN_OBJ)));
    pRecvThread->start();
}

bool CanBus::startCAN()
{
    if(VCI_StartCAN(m_deviceType,m_deviceIndex,m_canIndex)==STATUS_OK)
    {
        this->startRecvThread();
        return true;
    }
    else
        return false;
}

bool CanBus::closeCAN()
{
    if( VCI_CloseDevice(m_deviceType,m_deviceIndex) == STATUS_OK)
    {
        openFlag = false;
        return true;
    }
    else
        return false;
}

bool CanBus::isOpened()
{
    return openFlag;
}

bool CanBus::sendCommand(QString cmdData)
{

    QStringList cmdList = cmdData.split(" ");     //以空格分�?

    VCI_CAN_OBJ FrameInfo;
    FrameInfo.DataLen = 7; // 数据长度�?7

    if(cmdList.size() == 7)
    {
        bool ok = true;
        for(int i=0;i< 7;i++)
        {
            FrameInfo.Data[i] = cmdList[i].toInt(&ok,16);;
        }
        qDebug()<<FrameInfo.Data[1];

        FrameInfo.ID= 0x01;  //获取帧ID

        FrameInfo.SendType = 0;   //正常发�?
        FrameInfo.RemoteFlag = 0; //数据�?
        FrameInfo.ExternFlag = 0; //标准�?

        if( sendFrame(FrameInfo) )
            return true;
        else
            return false;
    }
    else
        return false;
}

bool CanBus::sendFrame(VCI_CAN_OBJ FrameInfo)
{
    if(VCI_Transmit(m_deviceType,m_deviceIndex,m_canIndex,&FrameInfo,1) == STATUS_OK)
        return true;
    else
        return false;
}

bool CanBus::readErrorInfo(VCI_ERR_INFO *pErrInfo)
{
    if(VCI_ReadErrInfo(m_deviceType,m_deviceIndex,-1,pErrInfo) == STATUS_OK)
        return true;
    else
        return false;
}

VCI_CAN_OBJ CanBus::getCurrentFrame()
{
    return currentFrame;
}

void CanBus::receiveFromCANThread(VCI_CAN_OBJ frameinfo)
{
    emit recvCANdata(frameinfo);
    currentFrame = frameinfo;
//    qDebug()<<"enter";
}
