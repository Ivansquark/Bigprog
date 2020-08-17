#ifndef BLOCK_BDKN_H
#define BLOCK_BDKN_H

//#include "gamma_bdg_measuring_measure.h"

#include "iblock.h"
//#include "neutron_measuring_measure.h"


class Block_BDKN:public Iblock
{
public:
    Block_BDKN(float temp=0,float p=0, float corrF=0,
               QString name=nullptr ,quint8 mesparam=0, quint8 diapparam=0);
    ~Block_BDKN();

    QString getBlockName(); //узнаем иимя блока
    float getDoseRate();    //узнаем величину измеряемого параметра
    quint8 getMesParam();   //узнаем номер измеряемого параметра (из него в дальнейшем узнаем размерность)
    quint8 getMesDiap();   //узнаем номер измеряемого параметра (из него в дальнейшем узнаем размерность)

private:
    void init();
    void doseRateCount(); //расчет доз (чувствительность умножить на скорость счета из Iblock-а)

    QString blockName{""}; //имя блока (БДКН-Т или БДКН-КА или БДКН-ПБ или БДКН-ГЕН)
    quint8 mesParam{0};    //параметр измерения (0-ПП, 1-МПД, 2-МАЭД)
    quint8 diapParam{0};   //параметр диапазона (в зависимости от него и параметра измерения выбирается чувствительность)
    float sensCurr{0};     //текущая чувствительность
    float doseRate;        //текущая мощность (или плотность потока)
};

#endif // BLOCK_BDKN_H
