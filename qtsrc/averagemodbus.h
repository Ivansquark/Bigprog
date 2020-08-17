#ifndef AVERAGEMODBUS_H
#define AVERAGEMODBUS_H

#include "modbus_emulator.h"
#include "password.h"
#include "rs485.h"

#include <QObject>
#include <QThread>
#include <QFile>
#include <QTextStream>

class AverageModbus: public QObject
{
    Q_OBJECT
public:
    AverageModbus();
    ~AverageModbus();

signals:
    void finish();
    void errorSig();

public slots:
    void recieveData(float countRate);
    void errorConnectionSlot();


public:
    float getAvCR();

private:
    Modbus_emulator* modEmul{nullptr};
    RS485* rs_485{nullptr};
    QThread* thread{nullptr};            //объект класса управления потоками (для многопоточности)
// многопоточность нужна будет для того чтобы процесс передачи данных с реального детектора находился в отдельном потоке
// чтобы несмотря ни на какие глюки в основном потоке отдельный поток соединения с детектором работал сам по себе
// эмулятор сделан через отдельный поток просто для тренировки
    QFile* file{nullptr};
    QTextStream* stream{nullptr};

    void init();
    void emul(); // метод вызова эмулятора
    void rs485(); // метод вызова детектора через RS-485
    //void average(qfloat16);
    void callAverCR(float); //метод вызова рассчета средней скорости счета


    float avCR{0}; //переменная хранящая в себе среднюю скорость счета


    bool CurrSource{0}; //переменная хранящая состояние выбора источниеа измерение (0-эмулятор, 1-детектор)
    float avData{0}; //вспомогательная переменная - используется для усреднения
    quint8 times{0}; //количество усреднений
    quint8 count{0}; //отсчеты для усреднения
};

#endif // AVERAGEMODBUS_H
