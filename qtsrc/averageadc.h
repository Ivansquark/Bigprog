#ifndef AVERAGEADC_H
#define AVERAGEADC_H

#include "emulelectrometer.h"
#include "rs232.h"

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
//#include <QTimer>

class AverageADC: public QObject
{
    Q_OBJECT
public:
    AverageADC();
    ~AverageADC();    

    long getAverageADC(); //метод из которой можно узнать текущее усредненное показание АЦП


public slots:
    void recieveData(QString);
    void sendToComSlot(QByteArray);
    void sendToComSlot1(QByteArray);

private:
    void init();
    void emul();  //метод вызова объекта класса эмулятора (электрометра) и старт эмулятора
    void rs_232();
    void callAverADC(long); //метод записи среднего значения в переменную averageADC

    EmulElectrometer* em{nullptr}; //указатель на класс эмулятора электрометра
    RS232* rs232{nullptr};

    uint8_t times; //количество отсчетов при усреднении
    long avADC{0};  //переменная используемая при усреднении
    bool CurrSource{0}; //текущий источник сигнала - эмулятор или детектор
    int count{0};  //переменная используемая для счета
    long averageADC{0}; //среднее показание с АЦП
};

#endif // AVERAGEADC_H
