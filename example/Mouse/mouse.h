#ifndef MOUSE_H
#define MOUSE_H

#include <QMainWindow>
#include <QThread>

class Mouse : public QThread
{
    Q_OBJECT
public:
    Mouse(QObject *parent=0);
    ~Mouse();
public:
    long n1;
    long n2;
    long n3;
    long n4;
    long n5;
    long n6;
protected:
    void run();
signals:
    void UpdateSignal(long,long,long,long,long,long);
public slots:
    void ResetSlot();
};

#endif // MOUSE_H
