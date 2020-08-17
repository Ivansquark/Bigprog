#ifndef GAMMAIK_CHAMBER_H
#define GAMMAIK_CHAMBER_H

#include <cmath>
#include "ichamber.h"

class GammaIK_chamber: public Ichamber
{   
public:
    GammaIK_chamber(uint8_t r=0, bool comp=0,double temp=0,double p=0,
                    double corrF=0, QString chamName="",quint8 mesparam=0);
    ~GammaIK_chamber();

public:
    double getDoseRate(); //узнаем величину измеряемого параметра
    QString getBlockName(); //узнаем иимя камеры
    quint8 getMesParam();   //узнаем номер измеряемого параметра (из него в дальнейшем узнаем размерность)

private:
    void init();
    void doseRateCount();             //метод из которого можно узнать текущее значение мощности дозы.

    QString chamberName{""}; // имя камеры из которого узнаем чувствительность
    //double sensibility;  // чувствительность
    quint8 mesParam{0};
    double sensCurr{0};     //текущая чувствительность
    double doseRate{0};
};

#endif // GAMMAIK_CHAMBER_H
