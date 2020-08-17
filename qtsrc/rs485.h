#ifndef RS485_H
#define RS485_H


#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QTimer>
#include <QDebug>

#include <QModbusClient>
#include <QModbusDevice>
#include <QModbusRtuSerialMaster>
#include <QModbusDataUnit>

#include <QMessageBox>

#include <qfloat16.h>


class RS485:public QObject
{
    Q_OBJECT
public:
    explicit RS485(QObject* parent=nullptr);
    ~RS485();
    //QSerialPort *port;// = new QSerialPort(this);


    static bool BDGorBDKN; //BDG=0 BDKN=1

    void start();
    void stop();

    QModbusDataUnit readRequest() const;
    QModbusDataUnit writeRequest() const;

signals:
    void signalData(float countRate);
    //qfloat16 signalQFloat(qfloat16);
    void errorSig();


public slots:
    void doWork();
    void endWork();

private slots:
    void timeOut();
    void error();           //ошибка соединения
    void onStateChanged(quint8);  //статус соединения

    void readyRead();

private:
    void init();

    void ConnectPort();
    void DisconnectPort();
    void WriteToRegister();
    void ReadPort();

    void callError();


    QModbusClient *modbusDevice{nullptr};//QModbusClient QModbusRtuSerialMaster
    QTimer *timer{nullptr};
    //QTimer *timerDelay = new QTimer(this);//таймер задержки
    QMessageBox *msg{nullptr};


    quint8 SERVER_ADDRESS=0;// адрес сервера (MASTER)
    //int START_ADDRESS = 0;       // адрес регистра
    //int AMOUNT = 4;              // количество регистров

    quint16 START_ADDRESS = 0;       // адрес регистра скорости счета ТОРОПЫ
    quint8 AMOUNT = 0;              // количество регистров скорости счета ТОРОПЫ - 2 для float

    const int WRITE_SIZE = 1;    // количество перезаписываемых регистров

    const int BAUDS = 57600;
    const int STOP_BITS = 1;
    const int DATA_BITS = 8;
    const QString PORT = "/dev/ttyUSB0";
    const int PARITY = QSerialPort::NoParity;
    const int RESPONSE_TIME = 1000;

};
//bool RS485::BDGorBDKN=0;

#endif // RS485_H
