#ifndef BETA_CHAMBER_H
#define BETA_CHAMBER_H

#include "ichamber.h"
#include <cmath>

class Beta_chamber:public Ichamber
{
public:
    Beta_chamber(uint8_t r=0, bool comp=0,double temp=0,double p=0, double corrF=0, QString chamName=""); //конструктор по умолчанию
    ~Beta_chamber();
    double MPD();             //метод из которого можно узнать текущее значение мощности дозы.
    QString getChamberName(); //метод из которого можно узнать имя текущей камеры

private:
    void init();

    QString chamberName; // имя камеры из которого узнаем чувствительность
    double sensibility;  // чувствительность   
};

#endif // BETA_CHAMBER_H
