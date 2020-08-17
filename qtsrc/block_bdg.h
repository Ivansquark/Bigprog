#ifndef BLOCK_BDG_H
#define BLOCK_BDG_H

#include "iblock.h"
//#include "gamma_bdg_measuring_measure.h"


class Block_BDG : public Iblock
{
public:
    Block_BDG(float temp=0,float p=0, float corrF=0, QString name="" ,quint8 mesparam=0);
    ~Block_BDG();

    QString getBlockName(); //узнаем иимя блока
    float getDoseRate();    //узнаем величину измеряемого параметра
    quint8 getMesParam();   //узнаем номер измеряемого параметра (из него в дальнейшем узнаем размерность)

private:
    void init();
    void doseRateCount();

    QString blockName{""}; //имя блока (Америций или Цезий или Кобальт)
    quint8 mesParam{0};    //параметр измерения (0-МАЭД, 1-МНЭД, 2-МИЭД, 3-КЕРМА, 4-МПД, 5-МЭД)
    float sensCurr{0};     //текущая чувствительность

    float doseRate;
};

#endif // BLOCK_BDG_H
