//*********************** полу абстрактный класс реализует корректировку скорости счета в зависимости от давления и температуры ********
#include "iblock.h"

Iblock::Iblock(float temp,float p, float corrF):
    Temp(temp),P(p),CorrF(corrF)
{
    init();
    //P=1;
}
Iblock::~Iblock()
{
    qDebug()<<"~Iblock()";
    timer->stop();
    qDebug()<<"timer->stop();";
    delete timer;
    qDebug()<<"delete timer;";
    delete averageMod;
    qDebug()<<"delete averageMod;";
}



void Iblock::init()
{
    averageMod = new AverageModbus;
    connect(averageMod,&AverageModbus::errorSig,this,&Iblock::errorSlot);
    timer = new QTimer;
    connect(timer,&QTimer::timeout,this,&Iblock::timeOut);
    timer->start(500);    
}

void Iblock::timeOut() //раз в полсекунды рассчитываем значение скорректированной скорости счета
{
    account_CR();
}


void Iblock::account_CR()
{
    float CR = averageMod->getAvCR();
    IblockCR_Res = (CR*CorrF*760*((273+Temp)/(293*P))); //поправки на температуру и давление и корректирующий коэффициент
    qDebug()<<IblockCR_Res;
}

void Iblock::errorSlot()
{
    emit errorSig();
}
