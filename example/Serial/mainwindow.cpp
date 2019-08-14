#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QtSerialPort/QSerialPortInfo>
#include <qdebug.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->pushButton_2->setEnabled(false);
    ui->pushButton_3->setEnabled(false);
    ui->pushButton_5->setEnabled(false);
    ui->pushButton_6->setEnabled(false);
    ui->pushButton_8->setEnabled(false);
    ui->groupBox_3->setEnabled(false);
    ui->groupBox_5->setEnabled(false);
    ui->groupBox_6->setEnabled(false);

    serial = new QSerialPort(this);
    serial2 = new QSerialPort(this);

    timer = new QTimer();

    initialise_p();
    initialise_t();

    connect(serial, &QSerialPort::readyRead, this, &MainWindow::readData_from_COM1);
    connect(serial2, &QSerialPort::readyRead, this, &MainWindow::readData_from_COM2);
    connect(timer, SIGNAL(timeout()), this, SLOT(writeData_to_COM2()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openSerialPort1()
{
    serial->setPortName(p.name);
    serial->setBaudRate(p.baudRate);
    serial->setDataBits(p.dataBits);
    serial->setParity(p.parity);
    serial->setStopBits(p.stopBits);
    serial->setFlowControl(p.flowControl);
    if (serial->open(QIODevice::ReadWrite))
    {//以读和写方式打开串口
        QString str = p.name+" has been opened";
        qDebug() << str;
        ui->pushButton_2->setEnabled(true);
        ui->pushButton_3->setEnabled(true);
        ui->pushButton->setEnabled(false);
        if(ui->pushButton_4->isEnabled()==0)
        {
            ui->groupBox_3->setEnabled(true);
            ui->groupBox_5->setEnabled(true);
            ui->groupBox_6->setEnabled(true);
        }
        return;
    }
    else
    {
        QMessageBox::critical(this, tr("Error"), serial->errorString());
    }
}

void MainWindow::openSerialPort2()
{
    serial2->setPortName(t.name);
    serial2->setBaudRate(t.baudRate);
    serial2->setDataBits(t.dataBits);
    serial2->setParity(t.parity);
    serial2->setStopBits(t.stopBits);
    serial2->setFlowControl(t.flowControl);
    if (serial2->open(QIODevice::ReadWrite))
    {//以读和写方式打开串口
        QString str = t.name+" has been opened";
        qDebug() << str;
        ui->pushButton_5->setEnabled(true);
        ui->pushButton_6->setEnabled(true);
        ui->pushButton_4->setEnabled(false);
        if(ui->pushButton->isEnabled()==0)
        {
            ui->groupBox_3->setEnabled(true);
            ui->groupBox_5->setEnabled(true);
            ui->groupBox_6->setEnabled(true);
        }
        return;
    }
    else
    {
        QMessageBox::critical(this, tr("Error"), serial2->errorString());
    }
}

void MainWindow::closeSerialPort1()
{
    if (serial->isOpen())
    {
        serial->close();
        QString str = p.name+" has been closed";
        qDebug() << str;
        ui->pushButton->setEnabled(true);
        ui->pushButton_2->setEnabled(false);
        ui->pushButton_3->setEnabled(false);
        ui->groupBox_3->setEnabled(false);
        ui->groupBox_5->setEnabled(false);
        ui->groupBox_6->setEnabled(false);
    }
}

void MainWindow::closeSerialPort2()
{
    if (serial2->isOpen())
    {
        serial2->close();
        QString str = t.name+" has been closed";
        qDebug() << str;
        ui->pushButton_4->setEnabled(true);
        ui->pushButton_5->setEnabled(false);
        ui->pushButton_6->setEnabled(false);
        ui->groupBox_3->setEnabled(false);
        ui->groupBox_5->setEnabled(false);
        ui->groupBox_6->setEnabled(false);
    }
}

void MainWindow::writeData_to_COM1()
{
    QByteArray text = ui->lineEdit->text().toUtf8();
    text = toHex(text);
    serial->write(text);
    qDebug() << text;
}

void MainWindow::writeData_to_COM1(QByteArray senddata)
{
    serial->write(senddata);
    qDebug() << senddata;
}

void MainWindow::writeData_to_COM2()
{
    QByteArray text = ui->lineEdit_2->text().toUtf8();
    text = toHex(text);
    serial2->write(text);
    qDebug() << text;
}

void MainWindow::readData_from_COM1()
{
    QByteArray data = serial->readAll();
    qDebug() << data;

    if(data.length() == 5)
    {
        int coordinate[3];
        coordinate[0] = data[1];
        coordinate[1] = data[2];
        coordinate[2] = data[3];

        QString str[3];
        str[0]="X:"+QString::number(coordinate[0]);
        str[1]="Y:"+QString::number(coordinate[1]);
        str[2]="Z:"+QString::number(coordinate[2]);
        ui->label->setText(str[0]);
        ui->label_2->setText(str[1]);
        ui->label_3->setText(str[2]);
    }
}

void MainWindow::readData_from_COM2()
{
    QByteArray data = serial2->readAll();
    qDebug() << data;

    show_Degree(data);

    int current_degree[3],delta_degree[3];
    if(data.at(4)/16 == 0)
        current_degree[0] = data[4]%16*100+data[5]/16*10+data[5]%16;
    else
        current_degree[0] = -1*(data[4]%16*100+data[5]/16*10+data[5]%16);
    if(data.at(7)/16 == 0)
        current_degree[1] = data[7]%16*100+data[8]/16*10+data[8]%16;
    else
        current_degree[1] = -1*(data[7]%16*100+data[8]/16*10+data[8]%16);
    if(data.at(10)/16 == 0)
        current_degree[2] = data[10]%16*100+data[11]/16*10+data[11]%16;
    else
        current_degree[2] = -1*(data[10]%16*100+data[11]/16*10+data[11]%16);

    if(!(degree[0]==0 && degree[1]==0 && degree[2]==0))
    {
        delta_degree[0] = current_degree[0]-degree[0];
        delta_degree[1] = current_degree[1]-degree[1];
        delta_degree[2] = current_degree[2]-degree[2];

        QString str;
        str = "##0"
                +QString::number(delta_degree[0]<0)+"0"
                +QString::number(abs(delta_degree[0]/20))+"0"
                +QString::number(delta_degree[1]<0)+"0"
                +QString::number(abs(delta_degree[1]/20))+"0"
                +QString::number(delta_degree[2]<0)+"0"
                +QString::number(abs(delta_degree[2]/80))
                +"%%0D0A";
        qDebug()<<str;
        QByteArray senddata = str.toUtf8();
        senddata = toHex(senddata);
        writeData_to_COM1(senddata);
    }
    if(degree[0]==0 && degree[1]==0 && degree[2]==0)
    {
        degree[0]=current_degree[0];
        degree[1]=current_degree[1];
        degree[2]=current_degree[2];
    }
}

 void MainWindow::initialise_p()
 {
     p.name = ui->lineEdit_3->text();
     p.baudRate = QSerialPort::Baud9600;
     p.dataBits = QSerialPort::Data8;
     p.parity = QSerialPort::NoParity;
     p.stopBits = QSerialPort::OneStop;
     p.flowControl = QSerialPort::NoFlowControl;
 }

 void MainWindow::initialise_t()
 {
     t.name = ui->lineEdit_4->text();
     t.baudRate = QSerialPort::Baud9600;
     t.dataBits = QSerialPort::Data8;
     t.parity = QSerialPort::NoParity;
     t.stopBits = QSerialPort::OneStop;
     t.flowControl = QSerialPort::NoFlowControl;
 }

 char MainWindow::ConvertHexChar(char ch)
 {
     if((ch >= '0') && (ch <= '9'))
     return ch-0x30;
     else if((ch >= 'A') && (ch <= 'F'))
     return ch-'A'+10;
     else if((ch >= 'a') && (ch <= 'f'))
     return ch-'a'+10;
     else return (-1);
 }

 QByteArray MainWindow::toHex(QByteArray data)
 {
     int len = data.length();
     QByteArray senddata;
     senddata.resize(len/2);
     int first,second;
     for(int i = 0;i < (len+1)/2;i++)
     {
         first = ConvertHexChar(data[2*i]);
         second = ConvertHexChar(data[2*i+1]);
         senddata[i] = first*16+second;
     }
     return senddata;
 }

 void MainWindow::show_Degree(QByteArray data)
 {
     double show_degree[3];
     if(data.at(4)/16 == 0)
         show_degree[0] = data[4]%16*100+data[5]/16*10+data[5]%16+(data[6]/16)*0.1+(data[6]%16*0.01);
     else
         show_degree[0] = -1*(data[4]%16*100+data[5]/16*10+data[5]%16+(data[6]/16)*0.1+(data[6]%16*0.01));
     if(data.at(7)/16 == 0)
         show_degree[1] = data[7]%16*100+data[8]/16*10+data[8]%16+(data[9]/16)*0.1+(data[9]%16*0.01);
     else
         show_degree[1] = -1*(data[7]%16*100+data[8]/16*10+data[8]%16+(data[9]/16)*0.1+(data[9]%16*0.01));
     if(data.at(10)/16 == 0)
         show_degree[2] = data[10]%16*100+data[11]/16*10+data[11]%16+(data[12]/16)*0.1+(data[12]%16*0.01);
     else
         show_degree[2] = -1*(data[10]%16*100+data[11]/16*10+data[11]%16+(data[12]/16)*0.1+(data[12]%16*0.01));

     QString str[3];
     str[0]="P:"+QString::number(show_degree[0]);
     str[1]="R:"+QString::number(show_degree[1]);
     str[2]="H:"+QString::number(show_degree[2]);
     ui->label_4->setText(str[0]);
     ui->label_5->setText(str[1]);
     ui->label_6->setText(str[2]);
 }

void MainWindow::on_pushButton_clicked()
{
    openSerialPort1();
}

void MainWindow::on_pushButton_2_clicked()
{
    closeSerialPort1();
}

void MainWindow::on_pushButton_3_clicked()
{
    writeData_to_COM1();
}

void MainWindow::on_pushButton_4_clicked()
{
    openSerialPort2();
}

void MainWindow::on_pushButton_5_clicked()
{
    closeSerialPort2();
}

void MainWindow::on_pushButton_6_clicked()
{
    writeData_to_COM2();
}

void MainWindow::on_pushButton_7_clicked()
{
    timer->start(500);
    ui->pushButton_7->setEnabled(false);
    ui->pushButton_8->setEnabled(true);
}

void MainWindow::on_pushButton_8_clicked()
{
    timer->stop();
    QString str = "##"+ui->center_x->text()+ui->center_y->text()+ui->r->text()+"%%0D0A";
    QByteArray senddata = str.toUtf8();
    senddata = toHex(senddata);
    writeData_to_COM1(senddata);
    ui->pushButton_7->setEnabled(true);
    ui->pushButton_8->setEnabled(false);
}

void MainWindow::on_lineEdit_3_editingFinished()
{
    initialise_p();
}

void MainWindow::on_lineEdit_4_editingFinished()
{
    initialise_t();
}

void MainWindow::on_pushButton_9_clicked()
{
    QString str = "DB07"+ui->center_x->text()+ui->center_y->text()+ui->r->text()+"%%0D0A";
    QByteArray senddata = str.toUtf8();
    senddata = toHex(senddata);
    senddata[2] = senddata[2]/16*10+senddata[2]%16;
    senddata[3] = senddata[3]/16*10+senddata[3]%16;
    senddata[4] = senddata[4]/16*10+senddata[4]%16;
    writeData_to_COM1(senddata);
}

void MainWindow::on_pushButton_10_clicked()
{
    QString str = "DB090D0A";
    QByteArray senddata = str.toUtf8();
    senddata = toHex(senddata);
    writeData_to_COM1(senddata);
}

void MainWindow::on_pushButton_11_clicked()
{
    QString str = "DB06"+ui->x1->text()+ui->y1->text()+ui->x2->text()+ui->y2->text()+"%%0D0A";
    QByteArray senddata = str.toUtf8();
    senddata = toHex(senddata);
    senddata[2] = senddata[2]/16*10+senddata[2]%16;
    senddata[3] = senddata[3]/16*10+senddata[3]%16;
    senddata[4] = senddata[4]/16*10+senddata[4]%16;
    senddata[5] = senddata[5]/16*10+senddata[5]%16;
    writeData_to_COM1(senddata);
}

void MainWindow::on_pushButton_12_clicked()
{
    QString str = "DB090D0A";
    QByteArray senddata = str.toUtf8();
    senddata = toHex(senddata);
    writeData_to_COM1(senddata);
}
