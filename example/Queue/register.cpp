#include "register.h"
#include "ui_register.h"

Register::Register(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Register)
{
    ui->setupUi(this);

    m=0;
    ui->calling->hide();
    ui->number->hide();
    ui->amount->hide();
    timer1 = new QTimer();
    timer2 = new QTimer();

    connect(timer1, SIGNAL(timeout()), this, SLOT(hidenumber()));
    connect(timer2, SIGNAL(timeout()), this, SLOT(hideamount()));
}

Register::~Register()
{
    delete ui;
}

void Register::hidenumber()
{
    ui->number->hide();
    timer1->stop();
}

void Register::hideamount()
{
    ui->amount->hide();
    timer2->stop();
}

void Register::call()
{
    int a;
    if(q.IsEmpty()==0)
    {
        a=q.CheckFirst();
        q.Pop();
    }
    else
        a=0;
    emit call_back(a);
    QString s="窗口当前空闲";
    ui->serving->setText(s);
    if(a!=0)
    {
        s="请第"+QString::number(a)+"号顾客前往窗口";
        ui->calling->setText(s);
        ui->calling->show();
    }
}

void Register::serve(int x)
{
    QString s="窗口当前服务号码:"+QString::number(x);
    ui->serving->setText(s);
    ui->calling->hide();
}

void Register::on_registrate_clicked()
{
    m=m+1;
    q.Push(m);
    QString s="您的排队号码:"+QString::number(m);
    ui->number->setText(s);
    ui->number->show();
    timer1->start(2000);
}

void Register::on_inquire_clicked()
{
    int a;
    a=q.CheckSize();
    QString s="当前等待人数:"+QString::number(a);
    ui->amount->setText(s);
    ui->amount->show();
    timer2->start(2000);
}

