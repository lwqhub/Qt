#include "extendedwindow.h"
#include "ui_extendedwindow.h"

Extendedwindow::Extendedwindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Extendedwindow)
{
    ui->setupUi(this);
}

Extendedwindow::~Extendedwindow()
{
    delete ui;
}

void Extendedwindow::on_pushButton_clicked()
{
    this->hide();
    emit e2w();
}

void Extendedwindow::receive(double n1,double n2)
{
    this->show();
    QString n=QString::number((n1+n2),'g',6);
    ui->textBrowser->setText(n);
}
