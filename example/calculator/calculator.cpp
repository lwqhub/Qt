#include "calculator.h"
#include "ui_calculator.h"

Calculator::Calculator(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Calculator)
{
    ui->setupUi(this);
}

Calculator::~Calculator()
{
    delete ui;
}

void Calculator::on_number1_textEdited(const QString &arg1)
{
    QString n1=ui->number1->text();
    num1=n1.toDouble();
}

void Calculator::on_switch_2_clicked()
{
    this->hide();
    emit w2e(Result1,Result2);
}

void Calculator::on_number2_textEdited(const QString &arg1)
{
    QString n2=ui->number2->text();
    num2=n2.toDouble();
}


void Calculator::on_number2_editingFinished()
{
    Result1=num1+num2;
    QString n3=QString::number(Result1,'g',6);
    ui->result->setText(n3);
}

void Calculator::on_number1_2_textEdited(const QString &arg1)
{
    QString n=ui->number1_2->text();
    num3=n.toDouble();
}

void Calculator::on_number2_2_textEdited(const QString &arg1)
{
    QString n=ui->number2_2->text();
    num4=n.toDouble();
}

void Calculator::on_number2_2_editingFinished()
{
    Result2=num3*num4;
    QString n=QString::number(Result2,'g',6);
    ui->result_2->setText(n);
}

void Calculator::receive()
{
    this->show();
}





