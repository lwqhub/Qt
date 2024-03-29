//******************************
//movej_tool(POS[6])
//相对于基坐标


#ifndef _UR_INTERFACE_H
#define _UR_INTERFACE_H

#include <Eigen/Eigen>    //使用Eigen矩阵计算库
#include <cstring>
#include <sstream>
#include "socket.h"
#include "modbus-tcp.h"
#pragma comment(lib, "libmodbus.lib")  
using namespace Eigen;

/* robot modes */
#define ROBOT_RUNNING_MODE 0
#define ROBOT_FREEDRIVE_MODE 1
#define ROBOT_READY_MODE 2
#define ROBOT_INITIALIZING_MODE 3
#define ROBOT_SECURITY_STOPPED_MODE 4
#define ROBOT_EMERGENCY_STOPPED_MODE 5
#define ROBOT_FATAL_ERROR_MODE 6
#define ROBOT_NO_POWER_MODE 7
#define ROBOT_NOT_CONNECTED_MODE 8
#define ROBOT_SHUTDOWN_MODE 9

/****************************************************************************************************
类名：UR_interface
功能：提供控制UR机器人的C++函数接口
时间：2015-05-12
外部依赖：1. libmodbus: modbus-tcp接口,用于获取UR机器人寄存器中的数据
		  2. Eigen:矩阵运算库
		  3. Socket:提供TCP及UDP通信功能
说明：如需完善功能请参考UR机器人编程手册或技术论坛自行修改
****************************************************************************************************/
class UR_interface
{
public:
	UR_interface();
	~UR_interface();
	
    bool connect_robot(const std::string ip,int port=30003); //连接UR机器人,参数为UR机器人IP地址和端口号,默认为30003端口

	bool isConnected();

	void movej_joint(double joint_angle[6],float a=3,float v=0.1,float t=0,float r=0);//移动到基座标系下指定位置,参数为关节角
	void movej_pose(double tcp_pose[6],float a=3,float v=0.1,float t=0,float r=0);    //移动到基座标系下指定位置,参数为TCP位置
	void movel_pose(double tcp_pose[6],float a=1.2,float v=0.1,float t=0,float r=0);  //移动到基座标系下指定位置,线性插值方式
	
	void movej_tool(double relative_pose[6],float a=1.2,float v=0.1,float t=0,float r=0);//相对于工具坐标系移动
	void movel_tool(double relative_pose[6],float a=1.2,float v=0.1,float t=0,float r=0);//相对于工具坐标系移动

    //void movej_relative_TCP(double relative_pose[6],float a=1.2,float v=0.1,float t=0,float r=0);//相对基座标移动，参数为TCP位置
    //void movej_relative_joint(double relative_pose[6],float a=1.2,float v=0.1,float t=0,float r=0);//相对基座标移动，参数为关节角
    //YB修改运功函数
    void yb_movep_tool(double relative_pose[6],float a=1.2,float v=0.1,float t=0,float r=0);//相对于工具坐标移动位置，单位m
    void yb_movep_TCP(double relative_pose[6],float a=1.2,float v=0.1,float t=0,float r=0);//相对于TCP移动距离,单位m,效果同上
    void yb_movej(double joint_angle[6],float a=1.2,float v=0.1,float t=0,float r=0);//旋转绝对关节角度，单位rad
    void yb_movej_relative(double joint_angle[6],float a=1.2,float v=0.1,float t=0,float r=0);//旋转相对关节角度，单位rad
    void yb_get_joint_angle(double joint_angle[6]);

    void go_home(float a=1,float v=0.5);    //UR5回零位

    void speedl(double tcp_speed[6],float a=2,float t=20);   //TCP速度指令
    void speedj(double joint_speed[6],float a=2,float t=20); //关节速度指令,单位是rad/s
	void stopl(float a=2);   //Decellerate tool speed to zero    a: tool acceleration(m/s^2)
	void stopj(float a=0.2); //Decellerate joint speeds to zero  a: tool acceleration(rad/s^2)

	void move_circle(double diameter,int count,float a=1,float v=0.1); //从当前位置开始绕Z轴圆周运动,参数为直径,单位m
	void move_spiral(double diameter,double pitch,double depth,float a=1,float v=0.1); //从当前位置开始沿工具坐标系Z轴螺旋运动,参数依次为：直径,螺距,深度。单位m
	
	void get_tcp_pos(double pos[6]);             //获取机器人TCP位置和姿态,单位是mm
	void get_joint_angle(double joint_angle[6]); //获取机器人6个关节角度,单位是弧度
	
	void set_speed(double ratio);  //设置速度比例
	void set_robot_mode(int mode); //设置机器人模式
	 int get_robot_mode();          //获得机器人当前模式

	void set_real(); //设置为真实机器人
	void set_sim();  //设置为模拟机器人

	void power_off_robot();       //关闭机器人及控制器
	int  is_security_stopped();   //是否安全停机,返回值：0为false,1为true
	int  is_emergency_stopped();  //是否紧急停止,返回值：0为false,1为true

    static void matrix_2_UR6params(const Matrix4d &m,double UR_6params[6]);    //4x4齐次矩阵转换为UR机器人内部TCP位置姿态参数
    static void matrix_2_axisangle(const Matrix3d &m, double UR_AxisAngle[3]); //3x3旋转矩阵转换为axis-angle
    static void UR6params_2_matrix(const double UR_6params[6],Matrix4d &m);    //TCP位置姿态参数转换为4x4齐次矩阵
    static void axisangle_2_matrix(const double UR_AxisAngle[3], Matrix3d &m); //axis-angle转换为3x3旋转矩阵
	static void print_matrix(const MatrixXd &m,std::string filename);   //将矩阵写入文本文件,用于调试或分析

    void teachmode();
    void set_tcp_pos(double pos[]);
private:
	modbus_t *mb;
	bool connectedFlag;
	SocketClient *sender;          //TCP客户端,一般通过30003端口给UR控制器发送字符串指令
	//SocketClient *dashboard_sender;//与DashBoard server通信：通过29999端口给UR面板GUI发送指令,如关机,加载程序等功能
};
#endif        
