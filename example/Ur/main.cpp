#include "ur_class_test.h"
#include <QApplication>
#include "UrAPI/UR_interface.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    UR_class_test w;
    w.show();

    return a.exec();
}
