#ifndef ICHAMBER_H
#define ICHAMBER_H

#include "averageadc.h"

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QTimer>

class Ichamber:public QObject
{
    Q_OBJECT
public:
    Ichamber(uint8_t r=0, bool comp=0,double temp=0,double p=0, double corrF=0); //конструктор по умолчанию
    ~Ichamber();



signals:
    void writeToComSig(const QByteArray &);

public slots:
    void setVoltageSlot(const QByteArray &); //слот установки высокого напряжения
private slots:
    void timeOut();
    void singleTimeOut();

public:
    double getIres();   //можно вызывать из наследника

private:
    AverageADC* averageADC{nullptr}; //указатель на объект класса AverageADC в котором проводится измерение (или эмулятор или детектор)
    QTimer* timer{nullptr};
    QTimer* singleTimer{nullptr};

    void init();
    void account_I(); //метод рассчета тока
    void sendToElectrometer();

    uint8_t iR{0}; //значение диапазона измерения тока (0-чувствительный (R3), 1-средний (R2), 2-грубый (R1))
    bool Comp;     //флаг учета компенсации
    double Temp{0};
    double P{0};
    double CorrF{0};
    double R1{0};
    double R2{0};
    double R3{0};
    double Rcurr{0};  //текущее значение резистора
    long Comp1{0};
    long Comp2{0};
    long Comp3{0};
    long CompCurr{0}; //текущее значение компенсации
    uint DAC1{0};
    QByteArray bytesDAC; //массив байтов заполняемый значениями ЦАПа и передаваемый вместе со значениями высокого
    long Iin{0};
    long nullADC{0x7FFFFF}; //половина от 2^24 (На Ацп приходит 2,5 В => оно считает от -2,5 В до 0 и от 0 до 2,5 В (от 0 до 0x7FFFFF и от 0x7FFFFF до 0xFFFFFF))
    double IchamberRes{0};  //результирующий ток в зависимости от давления и температуры    
    QByteArray hC2, // код реле - К1+К7+К8 - Чувствительный диапазон + фильтр шумов (С1 - 560пФ) (будем использовать всегда)
               h90, // код реле - К3+К8 - Средний диапазон (на среднем диапазоне фильтр шумов не подключается)
               hA0; // код реле - К5+К7+К8 - Грубый диапазон + фильтр шумов (С1 - 560пФ) (на грубом диапазоне фильтр шумов не отключается)

};

#endif // ICHAMBER_H
