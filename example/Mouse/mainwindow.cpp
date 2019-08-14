#include "mainwindow.h"




MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(tr("test event"));

    myThread = new Mouse;
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
    connect(myThread, SIGNAL(UpdateSignal(long,long,long,long,long,long)), this, SLOT(UpdateSlot(long,long,long,long,long,long)));
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

void MainWindow::UpdateSlot(long n1,long n2,long n3,long n4,long n5,long n6)
{
    QString s1="TX:"+QString::number(n1);
    QString s2="TY:"+QString::number(n2);
    QString s3="TZ:"+QString::number(n3);
    QString s4="RX:"+QString::number(n4);
    QString s5="RY:"+QString::number(n5);
    QString s6="RZ:"+QString::number(n6);

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
