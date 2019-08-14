#include "canRecvThread.h"
#include <QDebug>
#include "CANAPILibrary/ControlCAN.h"

CanRecvThread::CanRecvThread(QObject *parent) :
    QThread(parent)
{
    qRegisterMetaType<VCI_CAN_OBJ>("VCI_CAN_OBJ");      // 注册自定义类型

    m_deviceType = 4;
    m_deviceIndex = 0;
    m_canIndex = 0;

    stop = false;
}

CanRecvThread::~CanRecvThread()
{
    this->stopThread();
}

void CanRecvThread::init(int deviceType, int deviceIndex, int canIndex)
{
    m_deviceType = deviceType;
    m_deviceIndex = deviceIndex;
    m_canIndex = canIndex;
}
void CanRecvThread::run()
{
    VCI_CAN_OBJ frameinfo;
    VCI_ERR_INFO errinfo;

    int len=1;
    while(!stop)
    {
        QThread::msleep(1 );
        len=VCI_Receive(m_deviceType,m_deviceIndex,m_canIndex,&frameinfo,50,200);
        if(len<=0)
        {
            //注意：如果没有读到数据则必须调用此函数来读取出当前的错误码，
            //千万不能省略这一步（即使你可能不想知道错误码是什么）

            VCI_ReadErrInfo(m_deviceType,m_deviceIndex,m_canIndex,&errinfo);
        }
        else
        {
            emit recvdata(frameinfo);
//            qDebug()<<"receive";
        }
    }
}

void CanRecvThread::stopThread()
{
    stop = true;
}
