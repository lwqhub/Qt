#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    struct Settings {
        QString name;
        qint32 baudRate;
        QSerialPort::DataBits dataBits;
        QSerialPort::Parity parity;
        QSerialPort::StopBits stopBits;
        QSerialPort::FlowControl flowControl;
    };

    void openSerialPort1();
    void openSerialPort2();
    void closeSerialPort1();
    void closeSerialPort2();
    void writeData_to_COM1();
    void writeData_to_COM1(QByteArray senddata);
    void readData_from_COM1();
    void readData_from_COM2();

private slots:
    void writeData_to_COM2();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_lineEdit_3_editingFinished();

    void on_lineEdit_4_editingFinished();

    void on_pushButton_9_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_12_clicked();

private:
    Ui::MainWindow *ui;
    QSerialPort *serial,*serial2;
    Settings p;
    Settings t;
    QTimer *timer;
    int degree[3]={0,0,0};
    void initialise_p();
    void initialise_t();
    char ConvertHexChar(char ch);
    QByteArray toHex(QByteArray data);
    void show_Degree(QByteArray data);
};

#endif // MAINWINDOW_H
