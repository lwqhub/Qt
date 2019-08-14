#ifndef EXTENDEDWINDOW_H
#define EXTENDEDWINDOW_H

#include <QMainWindow>

namespace Ui {
class Extendedwindow;
}

class Extendedwindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit Extendedwindow(QWidget *parent = 0);
    ~Extendedwindow();

private slots:
    void on_pushButton_clicked();
    void receive(double,double);

signals:
    void e2w();

private:
    Ui::Extendedwindow *ui;
};

#endif // EXTENDEDWINDOW_H
