#ifndef CANBUS_H
#define CANBUS_H

#include <QObject>
#include "canRecvThread.h"
#include "CANAPILibrary/ControlCAN.h"

enum DEVICETYPE
{
    VCI_PCI5121_TYPE=1,
    VCI_PCI9810_TYPE=2,
    VCI_USBCAN1_TYPE=3,
    VCI_USBCAN2_TYPE=4,
    VCI_USBCAN2A_TYPE=4,
    VCI_PCI9820_TYPE=5,
    VCI_CAN232_TYPE=6,
    VCI_PCI5110_TYPE=7,
    VCI_CANLITE_TYPE=8,
    VCI_ISA9620_TYPE=9,
    VCI_ISA5420_TYPE=10,
    VCI_PC104CAN_TYPE=11,
    VCI_CANETE_TYPE=12,
    VCI_DNP9810_TYPE=13,
    VCI_PCI9840_TYPE=14,
    VCI_PC104CAN2_TYPE=15,
    VCI_PCI9820I_TYPE=16,
    VCI_CANETTCP_TYPE=17
};

class CanBus : public QObject
{
    Q_OBJECT
public:
    explicit CanBus(QObject *parent = 0);

    bool openCAN(DEVICETYPE deviceType, int deviceIndex, int canIndex);

    bool initCAN();
    bool initCAN(VCI_INIT_CONFIG CAN_Initstruct);

    void startRecvThread();

    bool startCAN();

    bool closeCAN();

    bool isOpened();

    bool sendCommand(QString cmdData);

    bool sendFrame(VCI_CAN_OBJ FrameInfo);

    bool readErrorInfo( VCI_ERR_INFO* pErrInfo);

    VCI_CAN_OBJ getCurrentFrame();

private:
    DEVICETYPE m_deviceType;
    int m_deviceIndex;
    int m_canIndex;

    bool openFlag;

    VCI_INIT_CONFIG m_CAN_Initstruct;

    CanRecvThread *pRecvThread;

    VCI_CAN_OBJ currentFrame;

signals:
    void recvCANdata(VCI_CAN_OBJ frameinfo);

public slots:
    void receiveFromCANThread(VCI_CAN_OBJ frameinfo);
};

#endif // CANBUS_H
