#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(tr("test event"));

    myThread = new Thread;
    showNum1 = new QLabel;
    showNum2 = new QLabel;
    showNum3 = new QLabel;
    showNum4 = new QLabel;
    showNum5 = new QLabel;
    showNum6 = new QLabel;
    startButton = new QPushButton(tr("start"));
    stopButton = new QPushButton(tr("stop"));
    resetButton = new QPushButton(tr("reset"));

    //连接各自信号糟
    connect(stopButton, SIGNAL(clicked()), this, SLOT(StopSlot()));
    connect(startButton, SIGNAL(clicked()), this, SLOT(StartSlot()));
    connect(resetButton, SIGNAL(clicked()), this, SLOT(ClearSlot()));
    connect(myThread, SIGNAL(UpdateSignal(int,int,int,int,int,int)), this, SLOT(UpdateSlot(int,int,int,int,int,int)));
    connect(this, SIGNAL(ResetSignal()), myThread, SLOT(ResetSlot()));


//    myThread->start();

    QVBoxLayout *vbox = new QVBoxLayout;

    vbox->addWidget(showNum1);
    vbox->addWidget(showNum2);
    vbox->addWidget(showNum3);
    vbox->addWidget(showNum4);
    vbox->addWidget(showNum5);
    vbox->addWidget(showNum6);
    vbox->addWidget(startButton);
    vbox->addWidget(stopButton);
    vbox->addWidget(resetButton);

    QWidget *centerWindow = new QWidget(this); // 用户在mainwindow类中添加自定义的控件和布局
    this->setCentralWidget(centerWindow);
    centerWindow->setLayout(vbox);
    centerWindow->setMinimumSize( 100, 120 );
}

void MainWindow::StartSlot()
{
    myThread->start();
}

void MainWindow::StopSlot()
{
    myThread->terminate();
}

void MainWindow::UpdateSlot(int num1,int num2,int num3,int num4,int num5,int num6)
{
    QString s1="TX:"+QString::number(num1);
    QString s2="TY:"+QString::number(num2);
    QString s3="TZ:"+QString::number(num3);
    QString s4="RX:"+QString::number(num4);
    QString s5="RY:"+QString::number(num5);
    QString s6="RZ:"+QString::number(num6);

    showNum1->setText(s1);
    showNum2->setText(s2);
    showNum3->setText(s3);
    showNum4->setText(s4);
    showNum5->setText(s5);
    showNum6->setText(s6);
}

void MainWindow::ClearSlot()
{
    emit ResetSignal();
}

MainWindow::~MainWindow()
{

}

Thread::Thread(QObject *parent)
{
    number1 = 0;
    number2 = 1;
    number3 = 3;
    number4 = 4;
    number5 = 5;
    number6 = 6;
}

Thread::~Thread()
{
}

void Thread::run()
{
    while(1)
    {
        //开启一个死循环，让number每秒自增1，并通过发送UpdateSignal信号通知主界面更新
        emit UpdateSignal(number1,number2,number3,number4,number5,number6);
        number1++;
        number2++;
        number3++;
        number4++;
        number5++;
        number6++;
        sleep(1);
    }
}

void Thread::ResetSlot()
{
    number1 = 0;
    number2 = 0;
    number3 = 0;
    number4 = 0;
    number5 = 0;
    number6 = 0;
    emit UpdateSignal(number1,number2,number3,number4,number5,number6);
}
