
#include "MotorControl.h"
#include <QDebug>
MotorControl::MotorControl(void)
{
    pCAN = new CanBus();
    connect(pCAN,SIGNAL(recvCANdata(VCI_CAN_OBJ)),this,SLOT(recvfromCAN(VCI_CAN_OBJ)));\

    getDataTimer = new QTimer();
    connect(getDataTimer,SIGNAL(timeout()), this, SLOT(getDataSlot()) );

    realTimeDataFlag = NONE;

    lastspeed = 0;

    openFlag = false;
    for(int i = 0; i < 5;i++)
    {
        motorSpeed[i] = 5000.0;
    }
}


MotorControl::~MotorControl(void)
{
    delete pCAN;
    delete getDataTimer;
}


int MotorControl::OpenCanDevice()
{
    if(pCAN->isOpened())                //if CAN opened
    {
        pCAN->closeCAN();
    }

    int deviceIndex = 0;    //can??????
    int canIndex = 0;       //???·can

    if(!pCAN->openCAN(VCI_USBCAN2A_TYPE,deviceIndex,canIndex))
        return 0;
    else if(!pCAN->initCAN())
        return 1;
    else if(!pCAN->startCAN())
        return 2;
    else
    {
        openFlag = true;
        return 3;
    }

}

bool MotorControl::isOpened()
{
    return openFlag;
}

bool MotorControl::CloseCanDevice()
{
    qDebug()<<"close can";
    return pCAN->closeCAN();
}

bool MotorControl::SetMode(int motorID, int nMode)
{

    //VCI_CAN_OBJ??????CAN???????1????????????????????
    //VCI_CAN_OBJ??????VCI_Transmit??VCI_Receive???????????????CAN??????
    VCI_CAN_OBJ vco[1]; // ???????????????

    vco[0].ID = motorID; // ??????ID
    vco[0].SendType = 0;    // ????????(??????????=0???????????=1??????????=2??????????=3????????????????????????????????)
    vco[0].RemoteFlag = 0;  // ????? ,????????????????
    vco[0].ExternFlag = 0;  // ????
    vco[0].DataLen = 8;     // ???????8?????(???????(<=8)????Data?????)

    // Data????????????,???????????????? + ????????????? + ????? + ???????
    vco[0].Data[0] = 0x08;    //???????,     1???
    vco[0].Data[1] = motorID;    //?????????,1???

    vco[0].Data[2] = 0x2A;    //???(??????0x002A),2???.       ???:???????????????????,????????
    vco[0].Data[3] = 0x00;

    if(nMode == 0)    //?????
    {
        vco[0].Data[4] = 0x00;
        vco[0].Data[5] = 0x00;
        vco[0].Data[6] = 0x00;
        vco[0].Data[7] = 0x00;
    }
    else if(nMode == 1) //?????
    {
        vco[0].Data[4] = 0x00;
        vco[0].Data[5] = 0x01;
        vco[0].Data[6] = 0x00;
        vco[0].Data[7] = 0x00;
    }else if (nMode == 2)	//?????
    {
        vco[0].Data[4] = 0x00;
        vco[0].Data[5] = 0x02;
        vco[0].Data[6] = 0x00;
        vco[0].Data[7] = 0x00;
    }
    if(pCAN->isOpened())
        return pCAN->sendFrame(vco[0]);
    else
        return false;
}


bool MotorControl::EmergencyStop(int motorID)
{
    VCI_CAN_OBJ vco[1];
    vco[0].ID = motorID;
    vco[0].SendType = 0;
    vco[0].RemoteFlag = 0;
    vco[0].ExternFlag = 0;
    vco[0].DataLen = 8;
    vco[0].Data[0] = 0x08;    //???????,
    vco[0].Data[1] = motorID;    //?????????(68)
    vco[0].Data[2] = 0x84;    //???(??????0x0084)
    vco[0].Data[3] = 0x00;
    vco[0].Data[4] = 0x00;
    vco[0].Data[5] = 0x00;
    vco[0].Data[6] = 0x00;
    vco[0].Data[7] = 0x00;

    if(pCAN->isOpened())
        return pCAN->sendFrame(vco[0]);
    else
        return false;
}


bool MotorControl::ClearEmergencyStop(int motorID)
{
    VCI_CAN_OBJ vco[1];
    vco[0].ID = motorID;
    vco[0].SendType = 0;
    vco[0].RemoteFlag = 0; //??????????0??????????1???????
    vco[0].ExternFlag = 0; //????????,0?????????????1???????
    vco[0].DataLen = 8;

    vco[0].Data[0] = 0x08;    //???????,
    vco[0].Data[1] = motorID;    //?????????(68)

    vco[0].Data[2] = 0x85;    //???(??????0x0085)
    vco[0].Data[3] = 0x00;

    vco[0].Data[4] = 0x00;
    vco[0].Data[5] = 0x00;
    vco[0].Data[6] = 0x00;
    vco[0].Data[7] = 0x00;

    if(pCAN->isOpened())
        return pCAN->sendFrame(vco[0]);
    else
        return false;
}

bool MotorControl::setHardwareLimit(int motorID, bool mode)
{
    VCI_CAN_OBJ vco[1];
    vco[0].ID = motorID;
    vco[0].SendType = 0;
    vco[0].RemoteFlag = 0; //??????????0??????????1???????
    vco[0].ExternFlag = 0; //????????,0?????????????1???????
    vco[0].DataLen = 8;

    vco[0].Data[0] = 0x08;    //???????,
    vco[0].Data[1] = motorID;    //?????????(68)

    vco[0].Data[2] = 0x41;    //???()
    vco[0].Data[3] = 0x00;

    if(mode)
        vco[0].Data[4] = 0x01;
    else
        vco[0].Data[4] = 0x00;

    vco[0].Data[5] = 0x00;
    vco[0].Data[6] = 0x00;
    vco[0].Data[7] = 0x00;

    if(pCAN->isOpened())
        return pCAN->sendFrame(vco[0]);
    else
        return false;
}


bool MotorControl::Stop(int motorID) //????"V0"???,????????0
{
    VCI_CAN_OBJ vco[1];
    vco[0].ID = motorID;
    vco[0].SendType = 0;
    vco[0].RemoteFlag = 0; //??????????0??????????1???????
    vco[0].ExternFlag = 0; //????????,0?????????????1???????
    vco[0].DataLen = 8;

    vco[0].Data[0] = 0x08;    //???????,
    vco[0].Data[1] = motorID;    //?????????(68)

    vco[0].Data[2] = 0x90;    //???(??????0x0090)
    vco[0].Data[3] = 0x00;

    vco[0].Data[4] = 0x00;
    vco[0].Data[5] = 0x00;
    vco[0].Data[6] = 0x00;
    vco[0].Data[7] = 0x00;

    if(pCAN->isOpened())
        return pCAN->sendFrame(vco[0]);
    else
        return false;
}


bool MotorControl::SetHome(int motorID)
{
    VCI_CAN_OBJ vco[1];
    //??ö???????????????????11?ID????1?????????????
    vco[0].ID = motorID;
    vco[0].SendType = 0;
    vco[0].RemoteFlag = 0;

    //??????????????????????????
    vco[0].ExternFlag = 0;
    vco[0].DataLen = 8;  //??????????

    //???????????????ÿ?????8????????????????????????
    vco[0].Data[0] = 0x08;    //???????,
    vco[0].Data[1] = motorID;    //?????????(68)
    vco[0].Data[2] = 0x98;    //???(??????0x0098)
    vco[0].Data[3] = 0x00;
    vco[0].Data[4] = 0x00;
    vco[0].Data[5] = 0x00;
    vco[0].Data[6] = 0x00;
    vco[0].Data[7] = 0x00;

    if(pCAN->isOpened())
        return pCAN->sendFrame(vco[0]);
    else
        return false;
}


bool MotorControl::MoveVelocity(int motorID,double vel)
{

    setHardwareLimit(motorID,true);


    VCI_CAN_OBJ vco[1];
    vco[0].ID = motorID;
    vco[0].SendType = 0;
    vco[0].RemoteFlag = 0;
    vco[0].ExternFlag = 0;
    vco[0].DataLen = 8;

    vco[0].Data[0] = 0x08;    //???????,
    vco[0].Data[1] = motorID;    //?????????(68)

    vco[0].Data[2] = 0x90;    //???(??????0x0090)
    vco[0].Data[3] = 0x00;

//    int rel=0;
//    if(motorID != motor_3)
//         rel = (int)(vel*43*60/360);   // K=512*27/1.25=11059.2;
//    else
//         rel = (int)(vel*43*60/3600);   // K=512*27/1.25=11059.2;


    int rel = (int)(vel*43*60/360);    //??/s   K//=60*27/1.25=1296;  //vel rad/s
//    qDebug()<<rel;
    vco[0].Data[4] = rel;
    vco[0].Data[5] = rel >> 8;
    vco[0].Data[6] = rel >> 16;
    vco[0].Data[7] = rel >> 24;

    if(pCAN->isOpened())
        return  pCAN->sendFrame(vco[0]);
    else
        return false;
}


bool MotorControl::MoveAbsolutePulse(int motorID,int distPulse,double val)
{
    setHardwareLimit(motorID,true);

    this->setHighestSpeed(motorID,val);  //?????????????

    VCI_CAN_OBJ vco[1];
    vco[0].ID = motorID;
    vco[0].SendType = 0;
    vco[0].RemoteFlag = 0;
    vco[0].ExternFlag = 0;
    vco[0].DataLen = 8;

    vco[0].Data[0] = 0x08;    //???????,
    vco[0].Data[1] = motorID;    //?????????(68)

    vco[0].Data[2] = 0x99;    //???(??????0x0099)
    vco[0].Data[3] = 0x00;

    //??????????????32????????
    vco[0].Data[4] = distPulse;
    vco[0].Data[5] = distPulse >> 8;
    vco[0].Data[6] = distPulse >> 16;
    vco[0].Data[7] = distPulse >> 24;

    if(pCAN->isOpened())
        return pCAN->sendFrame(vco[0]);
    else
        return false;
}




bool MotorControl::MoveRelative(int motorID,double dist,double vel)
{

    setHardwareLimit(motorID,true);

    this->setHighestSpeed(motorID,vel);  //?????????????

    VCI_CAN_OBJ vco[1];
    vco[0].ID = motorID;
    vco[0].SendType = 0;
    vco[0].RemoteFlag = 0;
    vco[0].ExternFlag = 0;
    vco[0].DataLen = 8;

    vco[0].Data[0] = 0x08;    //???????,
    vco[0].Data[1] = motorID;    //?????????(68)

    vco[0].Data[2] = 0x9A;    //???(??????0x009A)
    vco[0].Data[3] = 0x00;

    int rel=0;
    if(motorID != motor_4)
         rel = (int)(dist * 2048*43);   // K=512*27/1.25=11059.2;
    else
  //       rel = (int)(dist * 2048*157);   // K=512*27/1.25=11059.2;

    rel = (int)(dist * 246/360);   // K=512*27/1.25=11059.2;
    vco[0].Data[4] = rel;
    vco[0].Data[5] = rel >> 8;
    vco[0].Data[6] = rel >> 16;
    vco[0].Data[7] = rel >> 24;

    if(pCAN->isOpened())
        return pCAN->sendFrame(vco[0]);
    else
        return false;
}

bool MotorControl::MoveRelativePulse(int motorID, double pulse, double vel)
{
    setHardwareLimit(motorID,true);

    this->setHighestSpeed(motorID,vel);  //?????????????

    VCI_CAN_OBJ vco[1];
    vco[0].ID = motorID;
    vco[0].SendType = 0;
    vco[0].RemoteFlag = 0;
    vco[0].ExternFlag = 0;
    vco[0].DataLen = 8;

    vco[0].Data[0] = 0x08;    //???????,
    vco[0].Data[1] = motorID;    //?????????(68)

    vco[0].Data[2] = 0x9A;    //???(??????0x009A)
    vco[0].Data[3] = 0x00;


    int rel = (int)pulse;   //
    vco[0].Data[4] = rel;
    vco[0].Data[5] = rel >> 8;
    vco[0].Data[6] = rel >> 16;
    vco[0].Data[7] = rel >> 24;

    if(pCAN->isOpened())
        return pCAN->sendFrame(vco[0]);
    else
        return false;
}

bool MotorControl::MoveRelativeAngle(int motorID, double angle, double vel)
{
    setHardwareLimit(motorID,true);

    this->setHighestSpeed(motorID,vel);  //?????????????

    VCI_CAN_OBJ vco[1];
    vco[0].ID = motorID;
    vco[0].SendType = 0;
    vco[0].RemoteFlag = 0;
    vco[0].ExternFlag = 0;
    vco[0].DataLen = 8;

    vco[0].Data[0] = 0x08;    //???????,
    vco[0].Data[1] = motorID;    //?????????(68)

    vco[0].Data[2] = 0x9A;    //???(??????0x009A)
    vco[0].Data[3] = 0x00;


    int rel = (int)(angle*246*2000/360);   //LHS change for Hangtian Hospital    2018.07.12
    vco[0].Data[4] = rel;
    vco[0].Data[5] = rel >> 8;
    vco[0].Data[6] = rel >> 16;
    vco[0].Data[7] = rel >> 24;

    if(pCAN->isOpened())
        return pCAN->sendFrame(vco[0]);
    else
        return false;
}

bool MotorControl::MoveOnElectricity(int motorID,int electricity)
{
    VCI_CAN_OBJ vco[1];
    vco[0].ID = motorID;
    vco[0].SendType = 0;
    vco[0].RemoteFlag = 0;
    vco[0].ExternFlag = 0;
    vco[0].DataLen = 8;

    vco[0].Data[0] = 0x08;    //???????,
    vco[0].Data[1] = motorID;    //?????????

    vco[0].Data[2] = 0x96;    //???
    vco[0].Data[3] = 0x00;


    int rel = electricity;   //
    vco[0].Data[4] = rel;
    vco[0].Data[5] = rel >> 8;
    vco[0].Data[6] = rel >> 16;
    vco[0].Data[7] = rel >> 24;

    if(pCAN->isOpened())
        return pCAN->sendFrame(vco[0]);
    else
        return false;
}


bool MotorControl::speedObserver(int motorID, int speed)
{
    int product = lastspeed*speed;
    if(product == 0)
    {
        if(lastspeed == speed ) //all zero
        {
            lastspeed = 0;
            return false;
        }
        else if(lastspeed != 0 && speed == 0)
        {
            lastspeed = 0;
            return true;
        }
        else if(lastspeed == 0 && speed > 0)
        {
            lastspeed = 1;
            return true;
        }
        else if(lastspeed == 0 && speed < 0)
        {
            lastspeed = -1;
            return true;
        }
    }
    else if(product > 0 )   //???
    {
        if(speed > 0)
        {
            lastspeed = 1;
            return false;
        }
        else  // < 0
        {
            lastspeed = -1;
            return false;
        }
    }
    else if(product < 0)    //????
    {
        if(speed > 0)
        {
            lastspeed = 1;
            return true;
        }
        else  // < 0
        {
            lastspeed = -1;
            return true;
        }
    }
}

bool MotorControl::getElectricity(int motorID)
{
    VCI_CAN_OBJ vco[1];
    vco[0].ID = motorID;
    vco[0].SendType = 0;
    vco[0].RemoteFlag = 0;
    vco[0].ExternFlag = 0;
    vco[0].DataLen = 4;
    vco[0].Data[0] = 0x04;    //???????,
    vco[0].Data[1] = motorID;    //?????????
    vco[0].Data[2] = 0xD0;    //???
    vco[0].Data[3] = 0x00;

    if(pCAN->isOpened())
        return pCAN->sendFrame(vco[0]);
    else
        return false;
}

bool MotorControl::getRealSpeed(int motorID)
{
    VCI_CAN_OBJ vco[1];
    vco[0].ID = motorID;
    vco[0].SendType = 0;
    vco[0].RemoteFlag = 0;
    vco[0].ExternFlag = 0;
    vco[0].DataLen = 4;
    vco[0].Data[0] = 0x04;    //???????,
    vco[0].Data[1] = motorID;    //?????????
    vco[0].Data[2] = 0x91;    //???
    vco[0].Data[3] = 0x00;

    if(pCAN->isOpened())
        return pCAN->sendFrame(vco[0]);
    else
        return false;
}

bool MotorControl::getRealPosition(int motorID)
{
    VCI_CAN_OBJ vco[1];
    vco[0].ID = motorID;
    vco[0].SendType = 0;
    vco[0].RemoteFlag = 0;
    vco[0].ExternFlag = 0;
    vco[0].DataLen = 4;
    vco[0].Data[0] = 0x04;    //???????,
    vco[0].Data[1] = motorID;    //?????????
    vco[0].Data[2] = 0x9B;    //???
    vco[0].Data[3] = 0x00;

    if(pCAN->isOpened())
        return pCAN->sendFrame(vco[0]);
    else
        return false;
}

void MotorControl::addMotorID(int motorID)
{
    for(int i=0; i!=motorIdList.size(); ++i )      //???????????
       if ( motorIdList.at(i) == motorID)
           return;

    motorIdList.append(motorID);
}

void MotorControl::startRealTimeMonitoring(MonitorMode mode)
{
    this->setRealTimerMode(mode);
    getDataTimer->start(50);
}

void MotorControl::stopMonitoring()
{
    getDataTimer->stop();
}

void MotorControl::setRealTimerMode(MonitorMode mode)
{
    realTimeDataFlag = mode;
}

bool MotorControl::setHighestSpeed(int motorID,double vel)
{
    VCI_CAN_OBJ vco[1];
    vco[0].ID = motorID;
    vco[0].SendType = 0;
    vco[0].RemoteFlag = 0;
    vco[0].ExternFlag = 0;
    vco[0].DataLen = 8;

    vco[0].Data[0] = 0x08;    //???????,
    vco[0].Data[1] = motorID;    //?????????(68)

    vco[0].Data[2] = 0x16;    //???(??????0x009A)
    vco[0].Data[3] = 0x00;


    int rel = (int)(vel*43*60/360);   // K=512*27/1.25=11059.2;
//    qDebug()<<rel;
    vco[0].Data[4] = rel;
    vco[0].Data[5] = rel >> 8;
    vco[0].Data[6] = rel >> 16;
    vco[0].Data[7] = rel >> 24;

    if(pCAN->isOpened())
        return pCAN->sendFrame(vco[0]);
    else
        return false;
}
int MotorControl::bytesToInt(QByteArray bytes)
{
    if(bytes.size() == 2)
    {
        int addr = bytes[0] & 0x000000FF;
        addr |= ((bytes[1] << 8) & 0x0000FF00);
        return addr;
    }
    else if(bytes.size() == 4)
    {
        int addr = bytes[0] & 0x000000FF;
        addr |= ((bytes[1] << 8) & 0x0000FF00);
        addr |= ((bytes[2] << 16) & 0x00FF0000);
        addr |= ((bytes[3] << 24) & 0xFF000000);
        return addr;
    }
}

int MotorControl::getValueFromFrame(VCI_CAN_OBJ frameinfo )
{
    int value = 0;
    if(frameinfo.Data[0] == 8)
    {
        QByteArray data;
        data.resize(4);
        for(int i = 0; i < 4; i++)
            data[i] = frameinfo.Data[i+4];
        value =bytesToInt(data);
    }
    else if(frameinfo.Data[0] == 6)
    {
        QByteArray data;
        data.resize(2);
        for(int i = 0; i < 2; i++)
            data[i] = frameinfo.Data[i+4];
        value =bytesToInt(data);
    }
    return value;
}

void MotorControl::recvfromCAN(VCI_CAN_OBJ frameinfo)
{
    if(frameinfo.Data[2] == 0x91) //????
    {
        int speed = getValueFromFrame(frameinfo);
//        qDebug()<<"speed"<<speed;

        int motorID = frameinfo.Data[1];
        emit sendMonitorMsg(MonitorMsg(SPEED,motorID,speed));

        updataMortorSpeed(motorID,speed);
    }
    else if(frameinfo.Data[2] == 0xD0) //????
    {
        int electricity = getValueFromFrame(frameinfo);

        int ele = (int)moveMeanFilter(electricity);   //??????????

        int motorID = frameinfo.Data[1];
        emit sendMonitorMsg(MonitorMsg(ELECTRICITY,motorID,ele));
    }
    else if(frameinfo.Data[2] == 0x9B) //???
    {
        int position = getValueFromFrame(frameinfo);

        int motorID = frameinfo.Data[1];
        emit sendMonitorMsg(MonitorMsg(POSITION,motorID,position));
    }

    emit sendFrameinfo(frameinfo);
}
double MotorControl::meanFilter(QVector<double> dataVector)
{
    double sum = 0;
    for(int i = 0,iEnd = dataVector.size(); i <iEnd;i++ )
    {
        sum +=dataVector[i];
    }
    return sum/dataVector.size();
}

double MotorControl::moveMeanFilter(double newData)
{
    static QVector<double> dataVector(8);

    dataVector.erase(dataVector.begin());
    dataVector.push_back(newData);

    return meanFilter(dataVector);
}

void MotorControl::updataMortorSpeed(int motorID, double vel)
{
    switch(motorID)
    {
    case motor_1:
    {
        motorSpeed[0] = vel;
        break;
    }
    case motor_2:
    {
        motorSpeed[1] = vel;
        break;
    }
    case motor_3:
    {
        motorSpeed[2] = vel;
        break;
    }
    case motor_4:
    {
        motorSpeed[3] = vel;
        break;
    }
    case motor_5:
    {
        motorSpeed[4] = vel;
        break;
    }

    }
}

int MotorControl::getMortorCurSpeed(int motorID)
{
    switch(motorID)
    {
    case motor_1:
    {
        return motorSpeed[0];
        break;
    }
    case motor_2:
    {
//        qDebug()<<motorSpeed[1];
        return motorSpeed[1];
        break;
    }
    case motor_3:
    {
        return motorSpeed[2];
        break;
    }
    case motor_4:
    {
        return motorSpeed[3];
        break;
    }
    case motor_5:
    {
        return motorSpeed[4];
        break;
    }

    }
    return 0.0;
}


void MotorControl::getDataSlot()
{
    for(int i=0; i!=motorIdList.size(); ++i )      //???????????
    {
        uint motorID = motorIdList.at(i);
        switch(realTimeDataFlag)
        {
        case NONE:
        {
            break;
        }
        case ELECTRICITY:
        {
            getElectricity(motorID);
            break;
        }
        case SPEED:
        {
            getRealSpeed(motorID);
            break;
        }
        case POSITION:
        {
            getRealPosition(motorID);
            break;
        }
        case ELECTRICITY + SPEED:
        {
            getElectricity(motorID);
            getRealSpeed(motorID);
            break;
        }
        case ELECTRICITY + POSITION:
        {
            getElectricity(motorID);
            getRealPosition(motorID);
            break;
        }
        case SPEED + POSITION:
        {
            getRealSpeed(motorID);
            getRealPosition(motorID);
            break;
        }
        case ELECTRICITY+SPEED + POSITION:
        {
            getElectricity(motorID);
            getRealSpeed(motorID);
            getRealPosition(motorID);
            break;
        }

        }

        QThread::msleep(2);
    }
}

bool MotorControl::EnableMotor(int motorID)
{
    VCI_CAN_OBJ vco[1];
    vco[0].ID = motorID;
    vco[0].SendType = 0;
    vco[0].RemoteFlag = 0;
    vco[0].ExternFlag = 0;
    vco[0].DataLen = 8;
    vco[0].Data[0] = 0x08;    //???????,
    vco[0].Data[1] = motorID;    //?????????(68)
    vco[0].Data[2] = 0x80;    //???(??????0x0080)
    vco[0].Data[3] = 0x00;
    vco[0].Data[4] = 0x00;
    vco[0].Data[5] = 0x00;
    vco[0].Data[6] = 0x00;
    vco[0].Data[7] = 0x00;

    if(pCAN->isOpened())
        return pCAN->sendFrame(vco[0]);
    else
        return false;
}


bool MotorControl::DisableMotor(int motorID)
{
    VCI_CAN_OBJ vco[1];
    vco[0].ID = motorID;
    vco[0].SendType = 0;
    vco[0].RemoteFlag = 0;
    vco[0].ExternFlag = 0;
    vco[0].DataLen = 8;
    vco[0].Data[0] = 0x08;    //???????,
    vco[0].Data[1] = motorID;    //?????????(68)
    vco[0].Data[2] = 0x81;    //???(??????0x0081)
    vco[0].Data[3] = 0x00;
    vco[0].Data[4] = 0x00;
    vco[0].Data[5] = 0x00;
    vco[0].Data[6] = 0x00;
    vco[0].Data[7] = 0x00;

    if(pCAN->isOpened())
        return pCAN->sendFrame(vco[0]);
    else
        return false;
}


bool MotorControl::GetError(int motorID)
{
    VCI_CAN_OBJ vco[1];
    vco[0].ID = motorID;
    vco[0].SendType = 0;
    vco[0].RemoteFlag = 0;
    vco[0].ExternFlag = 0;
    vco[0].DataLen = 4;
    vco[0].Data[0] = 0x04;    //???????,
    vco[0].Data[1] = motorID;    //?????????(68)
    vco[0].Data[2] = 0xD2;    //???(??????0x00D2)
    vco[0].Data[3] = 0x00;

    if(pCAN->isOpened())
        return pCAN->sendFrame(vco[0]);
    else
        return false;
}
