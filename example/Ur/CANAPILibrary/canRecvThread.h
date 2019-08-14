#ifndef QTHREAD
#define QTHREAD
#include <QThread>
#include "CANAPILibrary/ControlCAN.h"
#include <QMetaType>

class MainWindow;
class CanRecvThread : public QThread
{
    Q_OBJECT
public:
    bool stop ;
    explicit CanRecvThread(QObject *parent = 0);
    ~CanRecvThread();
    void init(int deviceType, int deviceIndex, int canIndex );
    void run();
    void stopThread();

private:
    int m_deviceType;
    int m_deviceIndex;
    int m_canIndex;

signals:
    void recvdata(VCI_CAN_OBJ frameinfo);
public slots:

};
#endif // QTHREAD

