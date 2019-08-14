#ifndef SYSTEM_H
#define SYSTEM_H

#include <QMainWindow>
#include <QTimer>

namespace Ui {
class System;
}

class System : public QMainWindow
{
    Q_OBJECT

public:
    explicit System(QWidget *parent = 0);
    ~System();
    int n =0;//用于统计已服务人数（不包含叫号未到情况）
    int t =0;//用于记录当前服务号码
    QTimer *timer1;
    QTimer *timer2;
    QTimer *timer3;

signals:
    void call();

    void serve(int x);

private slots:
    void hidecalling();

    void hideserving();

    void hidecounting();

    void call_back(int x);

    void on_call_clicked();

    void on_serve_clicked();

    void on_count_clicked();

private:
    Ui::System *ui;
};

#endif // SYSTEM_H
