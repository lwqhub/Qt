#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QMainWindow>

namespace Ui {
class Calculator;
}

class Calculator : public QMainWindow
{
    Q_OBJECT

public:
    explicit Calculator(QWidget *parent = 0);
    ~Calculator();

private slots:
    void on_number1_textEdited(const QString &arg1);

    void on_switch_2_clicked();

    void on_number2_textEdited(const QString &arg1);

    void on_number2_editingFinished();

    void on_number1_2_textEdited(const QString &arg1);

    void on_number2_2_textEdited(const QString &arg1);

    void on_number2_2_editingFinished();

    void receive();

signals:
    void w2e(double,double);

private:
    Ui::Calculator *ui;
    QString input;
    double num1,num2,num3,num4,Result1,Result2;
};

#endif // CALCULATOR_H
