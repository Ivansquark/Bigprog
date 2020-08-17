#ifndef IBLOCK_H
#define IBLOCK_H

#include "averagemodbus.h"

#include <QObject>

class Iblock:public QObject
{
    Q_OBJECT
public:
    Iblock(float temp=0,float p=0, float corrF=0);
    ~Iblock();

signals:
    void errorSig();

public slots:
    void errorSlot();

private slots:
    void timeOut();
public:
    float IblockCR_Res{0};  //результирующая скорость счета в зависимости от давления и температуры
private:
    AverageModbus* averageMod{nullptr}; //указатель на объект класса AverageADC в котором проводится измерение (или эмулятор или детектор)
    QTimer* timer{nullptr};

    void init();
    void account_CR(); //метод рассчета скорости счета

    float Temp{0};
    float P{0};
    float CorrF{0};    
};

#endif // IBLOCK_H
