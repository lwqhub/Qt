#include "calculator.h"
#include "extendedwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Calculator w;
    Extendedwindow e;
    QObject::connect(&w,SIGNAL(w2e(double,double)),&e,SLOT(receive(double,double)));
    QObject::connect(&e,SIGNAL(e2w()),&w,SLOT(receive()));
    w.setFixedSize(500,500);
    e.setFixedSize(500,500);
    w.move(500,50);
    e.move(500,50);

    e.hide();
    w.show();

    return a.exec();
}
