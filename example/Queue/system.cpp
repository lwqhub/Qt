#include "system.h"
#include "ui_system.h"

System::System(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::System)
{
    ui->setupUi(this);
    ui->serve->setEnabled(false);
    ui->calling->hide();
    ui->serving->hide();
    ui->counting->hide();
    timer1 = new QTimer();
    timer2 = new QTimer();
    timer3 = new QTimer();

    connect(timer1, SIGNAL(timeout()), this, SLOT(hidecalling()));
    connect(timer2, SIGNAL(timeout()), this, SLOT(hideserving()));
    connect(timer3, SIGNAL(timeout()), this, SLOT(hidecounting()));
}

System::~System()
{
    delete ui;
}

void System::hidecalling()
{
    ui->calling->hide();
    timer1->stop();
}

void System::hideserving()
{
    ui->serving->hide();
    timer2->stop();
}

void System::hidecounting()
{
    ui->counting->hide();
    timer3->stop();
}

void System::call_back(int x)
{
    t=x;
    if(x==0)
    {
        QString s="目前无顾客排队！";
        ui->calling->setText(s);
        ui->calling->show();
        ui->serve->setEnabled(false);
        timer1->start(2000);
    }
    else
    {
        QString s="下一服务号码:"+QString::number(x);
        ui->calling->setText(s);
        ui->calling->show();
        timer1->start(2000);
        ui->serve->setEnabled(true);
    }
}

void System::on_call_clicked()
{
    emit call();
}

void System::on_serve_clicked()
{
    if(t!=0)
        n=n+1;
    QString s="当前服务号码:"+QString::number(t);
    ui->serving->setText(s);
    ui->serve->setEnabled(false);
    ui->serving->show();
    timer2->start(2000);
    emit serve(t);
}

void System::on_count_clicked()
{
    QString s="累计服务次数:"+QString::number(n);
    ui->counting->setText(s);
    ui->counting->show();
    timer3->start(2000);
}
