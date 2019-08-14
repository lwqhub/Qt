#include "system.h"
#include "register.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    System s;
    Register r;

    QObject::connect(&s,SIGNAL(call()),&r,SLOT(call()));
    QObject::connect(&r,SIGNAL(call_back(int)),&s,SLOT(call_back(int)));
    QObject::connect(&s,SIGNAL(serve(int)),&r,SLOT(serve(int)));

    s.move(750,200);
    r.move(250,200);

    s.show();
    r.show();

    return a.exec();
}
