QT       += core gui
QT	     += network
QT           += opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Ur
TEMPLATE = app

SOURCES += UrAPI/mythread.cpp \
    UrAPI/socket.cpp \
    UrAPI/ur_class_test.cpp \
    UrAPI/UR_interface.cpp \
    MotorControl.cpp \
    CANAPILibrary/canbus.cpp \
    CANAPILibrary/canRecvThread.cpp \
    main.cpp \
    3Dmouse/mouse.cpp

HEADERS  += UrAPI/mythread.h \
    UrAPI/socket.h \
    UrAPI/ur_class_test.h \
    UrAPI/UR_interface.h \
    dynamixel.h \
    MotorControl.h \
    CANAPILibrary/canbus.h \
    CANAPILibrary/ControlCAN.h \
    CANAPILibrary/canRecvThread.h \
    3Dmouse/3DxTEST32.H \
    3Dmouse/mouse.h \
    3Dmouse/resource.h \
    3Dmouse/stdafx.h \
    3Dmouse/targetver.h

FORMS    += UrAPI/ur_class_test.ui





#Ur
INCLUDEPATH +=  $$PWD\UrAPI\
                $$PWD\libmodbus\windows32\includes
LIBS +=  $$PWD\libmodbus\windows32\lib\libmodbus.lib

#eigen
INCLUDEPATH +=	$$PWD\eigen-eigen-07105f7124f9

#末端
#win32: LIBS += -L$$PWD/./ -ldynamixel

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.

win32: LIBS += -L$$PWD/CANAPILibrary/ -lControlCAN

INCLUDEPATH += $$PWD/CANAPILibrary
DEPENDPATH += $$PWD/CANAPILibrary

#Mouse
INCLUDEPATH += $$PWD\3Dmouse\Inc

LIBS += $$PWD\3Dmouse\Lib\x86\siapp.lib
