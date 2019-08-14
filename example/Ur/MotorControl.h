#pragma once

#include <QObject>
#include <CANAPILibrary/canbus.h>
#include <QTimer>

enum MOTORID
{
    motor_1 = 17,
    motor_2 = 18,
    motor_3 = 19,
    motor_4 = 20,
    motor_5 = 21,
};

enum MonitorMode
{
    NONE = 0,                   //����ȡֵ
    ELECTRICITY = 2,            //��ȡ����
    SPEED =3,                  //��ȡ�ٶ�
    POSITION =4,               //��ȡʵʱλ��
};

struct MonitorMsg
{
    MonitorMsg()
    {
        mode = NONE;
        value = 0;
        motorID = -1;
    }
    MonitorMsg(MonitorMode mode,int motorID, int value)
    {
        this->mode = mode;
        this->value = value;
        this->motorID = motorID;
    }
    MonitorMode mode;
    int value;
    int motorID;
};

class MotorControl:public QObject
{
    Q_OBJECT
public:
	MotorControl(void);
	~MotorControl(void);

    int OpenCanDevice(); //��ʼ��CAN  //����ֵ0- ����canʧ�ܣ�1-��ʼ��canʧ�ܣ�2-����canʧ�ܣ�3-�ɹ�

    bool isOpened();

    bool CloseCanDevice(); //�ر�CAN
	
	//��������ʹ��ǰӦ���úÿ���ģʽ
    bool SetMode(int motorID,int nMode);   //��������������ģʽ��0Ϊ�ٶ�ģʽ��1Ϊλ��ģʽ(��������),Ĭ��Ϊ�ٿ�ģʽ
	
    bool Stop(int motorID); //ֹͣ
    bool EmergencyStop(int motorID);               //������ͣ,�����ƶ�����,���������뼱ͣ״̬
    bool ClearEmergencyStop(int motorID);          //�����������ļ�ͣ״̬����������״̬

    bool setHardwareLimit(int motorID,bool mode);

    bool EnableMotor(int motorID);   //ʹ�ܿ��ƣ����������ص���
    bool DisableMotor(int motorID);  //�ѻ����ƣ��������ͷŵ���������������ת
	
	
    bool MoveVelocity(int motorID, double vel);     //�����������ٶ�,��λ��/s
    bool MoveRelative(int motorID,double dist,double vel);    //ֱ�߽�����������λ��,��λΪmm velΪ�ٶ�

    bool MoveRelativePulse(int motorID,double pulse,double vel);

    bool MoveRelativeAngle(int motorID, double angle, double vel);//LHS change    2018.07.12

    bool MoveOnElectricity(int motorID,int ELECTRICITY);   // ������ģʽ�ƶ�����λmA

    bool speedObserver(int motorID,int speed);      //�ٶȹ۲������۲⵱ǰֵ��֮ǰֵ�����Ƿ�һ��,һ��������0����һ������+1or-1
	
    bool SetHome(int motorID);   //���õ�����ǰλ��Ϊ������λ

	//������
    bool MoveAbsolutePulse(int motorID, int distPulse, double val); //����λ��ģʽ
    bool GetError(int motorID);                //��ȡ������Ϣ

    bool getElectricity(int motorID);		//��ȡ����ֵ
    bool getRealSpeed(int motorID);             //��ȡ�ٶ�ֵ
    bool getRealPosition(int motorID);        //��ȡʵʱλ��

    void addMotorID(int motorID);
    void startRealTimeMonitoring(MonitorMode mode); //����ʵʱ����
    void stopMonitoring();                          //stop

public slots:
    void getDataSlot();

private:
    void setRealTimerMode(MonitorMode mode);

    bool setHighestSpeed(int motorID,double vel);  //��λ: ��/s

private:
     int lastspeed;      //Ĭ��Ϊ��ֹ

     double meanFilter(QVector<double> dataVector);   //��ֵ
     double moveMeanFilter(double newData);
public:
    CanBus* pCAN;
    QTimer* getDataTimer;
    QList<uint> motorIdList;
    MonitorMode realTimeDataFlag;       //ʵʱ��ȡ��־��0������ȡ�κ�ֵ��1��ȡ�ٶ�ֵ��2��ȡ����ֵ��3��ȡ�ٶȺ͵���ֵ

    bool openFlag;

    float motorSpeed[5];

    void updataMortorSpeed(int motorID,double vel);
    int getMortorCurSpeed(int motorID);

    int bytesToInt(QByteArray bytes);
    int getValueFromFrame(VCI_CAN_OBJ frameinfo);
signals:
    void sendFrameinfo(VCI_CAN_OBJ frameinfo);
    void sendMonitorMsg(MonitorMsg monitorMsg);

private slots:
    void recvfromCAN(VCI_CAN_OBJ frameinfo);


};

