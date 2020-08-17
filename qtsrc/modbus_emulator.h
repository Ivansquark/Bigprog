#ifndef MODBUS_EMULATOR_H
#define MODBUS_EMULATOR_H


#include <QTimer>
#include <QTime>
#include <QDebug>
#include <qfloat16.h>
#include <cmath>
#include <random>

class Modbus_emulator: public QObject
{
    Q_OBJECT
public:
    Modbus_emulator();
    ~Modbus_emulator();
    void start();
    void stop();

signals:
    void modReg(float countRate);
    void finish();
public slots:
    void doWork(); //слоты для доступа из другого потока
    void endWork(); //слоты для доступа из другого потока
private slots:
    void timeOut();    

private:
    QTimer* timer;    

    void init();
    double gausRand(double MO, double sko); //метод возвращает случайное значение при передаче в него среднего и СКО
    //распределение случайной величины подчиняется нормальному распределению
    //ЦПТ сумма равномерных случайных величин - распределена по нормальному закону

    qfloat16 countsRate{0};
    qfloat16 deviation{0};
    QTime* time;
    std::default_random_engine* generator{nullptr};  //рандомный генератор из C++11
    std::normal_distribution<float>* distribution{nullptr}; //нормальное распределение из С++11

};

#endif // MODBUS_EMULATOR_H
