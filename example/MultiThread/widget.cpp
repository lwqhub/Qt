#include "widget.h"
#include <QDebug>
#include <windows.h>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    createView();

    /*关闭QWidget窗口，执行析构函数*/
    setAttribute(Qt::WA_DeleteOnClose);
}

void Widget::createView()
{
    /*添加界面*/
    mainLayout = new QVBoxLayout(this);
    QPushButton *openThreadBtn = new QPushButton(tr("打开线程"));
    QPushButton *closeThreadBtn = new QPushButton(tr("关闭线程"));
    mainLayout->addWidget(openThreadBtn);
    mainLayout->addWidget(closeThreadBtn);
    mainLayout->addStretch();
    connect(openThreadBtn,SIGNAL(clicked(bool)),this,SLOT(openThreadBtnSlot()));
    connect(closeThreadBtn,SIGNAL(clicked(bool)),this,SLOT(closeThreadBtnSlot()));

    qDebug()<<"主线程id:"<<QThread::currentThreadId();

}

void Widget::openThreadBtnSlot()
{
    /*开启一个线程*/
    thread1 = new MyThread;
    thread1->start();

    qDebug()<<tr("main QThread::currentThreadId()==")<<QThread::currentThreadId();
}

void Widget::closeThreadBtnSlot()
{
    /*关闭多线程*/
    thread1->closeThread();
    thread1->wait();
}

Widget::~Widget()
{
    thread1->wait();
    qDebug()<<tr("执行了析构函数");
}
