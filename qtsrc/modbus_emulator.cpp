#include "modbus_emulator.h"

Modbus_emulator::Modbus_emulator()
{
    init();
}

Modbus_emulator::~Modbus_emulator()
{
    delete timer;
    delete time;
    delete generator;
    delete distribution;    
}
void Modbus_emulator::init()
{
    timer=new QTimer(this);
    connect(timer,&QTimer::timeout,this,&Modbus_emulator::timeOut);    
    countsRate=400;                                 //выставляем скорость счета, которую необходимо менять для эмуляции измерения
    //deviation=countsRate*(float(5.0/100));//5%
    deviation=countsRate*(float((5.0/100)*(100/countsRate)));//const standard deviation
    time =new QTime;
    qsrand(static_cast<uint>(time->currentTime().second())); //рандомизируем рандомные значения

    //generator = new std::default_random_engine(static_cast<ulong>(time->currentTime().msec())); //С++ 11 вызов random
    //distribution=new std::normal_distribution<float>(countsRate,static_cast<float>(deviation)); //С++11 нормальное распределение (матожидание, дисперсия)

    //qfloat16 x=distribution->;
}

double Modbus_emulator::gausRand(double MO, double sko) //метод выдает случайное значение при передаче в него среднего и ско
{                         // по ЦПТ - суммы раномерно распределенных величин подчиняются нормальному распределению
    double sum=0, x=0;
    for (int i=0;i<25;i++)
        sum+=1.0*rand()/RAND_MAX;
    x = (sqrt(1.77)*(sko)*(sum-13.93))/(1.91) +sko+ MO;//хитрая формула вычисленная подбором и сравнением с std::normal_distribution
    return x;
}

void Modbus_emulator::start()
{
    timer->start(200);    
}

void Modbus_emulator::stop()
{
    timer->stop();
}

void Modbus_emulator::doWork() //слоты для доступа из другого потока
{
    //timer->start(200);
    start();   
}

void Modbus_emulator::endWork() //слоты для доступа из другого потока
{
    stop();
}

void Modbus_emulator::timeOut()
{    
    //countsRate=40+qrand()%(static_cast<int>(deviation))*std::pow(-1,qrand()%2); //равномерное распределение

    //std::default_random_engine generator(time->currentTime().msec());
    //std::normal_distribution<float> distribution(countsRate,deviation/2); //C++11, можно использовать или эту или gausRand()

    //qfloat16 x=distribution->operator()(*generator); //вызываем функтор нормального распределения, передаем в него рандомный генератор

    qfloat16 x=float(gausRand(double(countsRate),double(deviation))); //(получаем случайную величину, приводим ее к половинчатому float)
    //qDebug()<<x;
    //uint16_t* y =reinterpret_cast<uint16_t*>(&x); //переводим ее в двубайтовый int
    //quint8 xH=*y>>8;     //разбиваем на два байта - старший
    //quint8 xL=*y&0xFF;   //и младший (для передачи по ModBus RTU)
    //qDebug()<<xH<<xL;
    float countRate = x;
    emit modReg(countRate);  // посылаем сигнал с двумя байтами
}







