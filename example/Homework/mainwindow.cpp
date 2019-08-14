#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString str = ui->input->text();

    char* ch;
    QByteArray ba = str.toLatin1();
    ch=ba.data();//将QString转化为char

    while(*ch!='\0')
    {
        if(*ch>='0' && *ch<='9')
            s1.Push(*ch);
        else if(*ch=='+'||*ch=='-')
        {
            if(s2.IsEmpty()==1)
            {
                s2.Push(*ch);
            }
            else
            {
                while(s2.IsEmpty()!=1)
                {
                    calculate();
                }
                s2.Push(*ch);
            }
        }
        else if(*ch=='*'||*ch=='/')
        {
            if(s2.IsEmpty()==1)
            {
                s2.Push(*ch);
            }
            else
            {
                if(s2.vec.back()=='+'||s2.vec.back()=='-')
                {
                    s2.Push(*ch);
                }
                else if(s2.vec.back()=='*'||s2.vec.back()=='/')
                {
                    calculate();
                    s2.Push(*ch);
                }
            }
        }
        ch++;
    }
    while(s2.IsEmpty()!=1)
    {
        calculate();
    }

    ui->result->setText(QString::number(s1.vec[0]));
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->result->clear();
    ui->input->clear();
    s1.vec.clear();
    s2.vec.clear();
}

int MainWindow::calculate()
{
    double x1 = s1.Pop();
    double x2 = s1.Pop();
    char symbol = s2.Pop();
    if(symbol=='+')
        s1.Push(x2+x1);
    else if(symbol=='-')
        s1.Push(x2-x1);
    else if(symbol=='*')
        s1.Push(x2*x1);
    else if(symbol=='/')
        s1.Push(x2/x1);
    return 0;
}
