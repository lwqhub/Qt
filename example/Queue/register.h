#ifndef REGISTER_H
#define REGISTER_H

#include <QMainWindow>
#include <QTimer>
#include "queue.h"

namespace Ui {
class Register;
}

class Register : public QMainWindow
{
    Q_OBJECT

public:
    explicit Register(QWidget *parent = 0);
    ~Register();
    Queue q;
    int m;//记录取号次数
    QTimer *timer1;
    QTimer *timer2;

signals:
    void call_back(int x);

private slots:
    void hidenumber();

    void hideamount();

    void call();

    void serve(int x);

    void on_registrate_clicked();

    void on_inquire_clicked();

private:
    Ui::Register *ui;
};

#endif // REGISTER_H
