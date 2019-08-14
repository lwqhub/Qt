#include "control.h"
#include "ui_control.h"

Control::Control(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Control)
{
    ui->setupUi(this);

    mythread = new Joystick_Thread();

    connect(mythread, SIGNAL(UpdateSignal(DIJOYSTATE2)), this, SLOT(UpdateSlot(DIJOYSTATE2)));

    ui->b8->hide();
    ui->b9->hide();
    ui->b10->hide();
    ui->b14->hide();
    ui->b15->hide();
    ui->b16->hide();
}

Control::~Control()
{
    delete ui;
}

void Control::UpdateSlot(DIJOYSTATE2 js1)
{
    if (js1.rgbButtons[0] == 0x00)
    {
        QString strx = "Xdegree:"+QString::number(js1.lX);
        ui->label->setText(strx);
        QString stry = "Ydegree:"+QString::number(js1.lY);
        ui->label_2->setText(stry);
        QString strz = "Zdegree:"+QString::number(js1.lRz);
        ui->label_3->setText(strz);
    }
    else
    {
        QString strx = "Xdegree:"+QString::number(-js1.lX);
        ui->label->setText(strx);
        QString stry = "Ydegree:"+QString::number(-js1.lY);
        ui->label_2->setText(stry);
        QString strz = "Zdegree:"+QString::number(-js1.lRz);
        ui->label_3->setText(strz);
    }

    //Show_or_Hide_the_Button(js1);

    if(js1.rgbButtons[7] == 0x80)
        ui->b8->show();
    else
        ui->b8->hide();

    if(js1.rgbButtons[8] == 0x80)
        ui->b9->show();
    else
        ui->b9->hide();

    if(js1.rgbButtons[9] == 0x80)
        ui->b10->show();
    else
        ui->b10->hide();

    if(js1.rgbButtons[13] == 0x80)
        ui->b14->show();
    else
        ui->b14->hide();

    if(js1.rgbButtons[14] == 0x80)
        ui->b15->show();
    else
        ui->b15->hide();

    if(js1.rgbButtons[15] == 0x80)
        ui->b16->show();
    else
        ui->b16->hide();

}

void Control::on_pushButton_clicked()
{
    mythread->start();
}

void Control::on_pushButton_2_clicked()
{
    mythread->terminate();
}

void Control::Show_or_Hide_the_Button(DIJOYSTATE2 js1)
{
    if(state[7][0] == 0x00 && js1.rgbButtons[7] == 0x80)
    {
        if(state[7][1] == 0)
        {
            ui->b8->show();
            state[7][1] = 1;
        }
        else
        {
            ui->b8->hide();
            state[7][1] = 0;
        }
    }
    state[7][0] = js1.rgbButtons[7];

    if(state[8][0] == 0x00 && js1.rgbButtons[8] == 0x80)
    {
        if(state[8][1] == 0)
        {
            ui->b9->show();
            state[8][1] = 1;
        }
        else
        {
            ui->b9->hide();
            state[8][1] = 0;
        }
    }
    state[8][0] = js1.rgbButtons[8];
}
