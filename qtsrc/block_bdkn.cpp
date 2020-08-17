#include "block_bdkn.h"

Block_BDKN::Block_BDKN(float temp,float p, float corrF,
                        QString name,quint8 mesparam, quint8 diapparam):
    Iblock(temp, p,corrF),blockName(name),mesParam(mesparam),diapParam(diapparam)
{
    init();
}

Block_BDKN::~Block_BDKN()
{
    qDebug()<<"~Block_BDKN()";
}

void Block_BDKN::init()
{ 
    QString str = blockName;
    int x=str.indexOf(" ");
    blockName=str.left(x);
    float PP1=0;
    float PP2=0;
    float MPD1=0;
    float MPD2=0;
    float MAED1=0;
    float MAED2=0;
    if(QSqlDatabase::contains("myDB"))
    {
        QSqlDatabase db = QSqlDatabase::database("myDB");
        db.setDatabaseName("config.db");
        if(!db.open()){qDebug()<<"db was not opened";}
        QSqlQuery query(db);
        QString qStr="SELECT *FROM neutronBlocks WHERE name='%1'";
        QString qStr1=qStr.arg(blockName);
        if(!query.exec(qStr1)){qDebug()<<"unable execute query SELECT sensibility FROM neutronBlocks WHERE name='%1'";}
        if (query.first())
        {
            PP1=query.value("sensibilityPPwithoutCd").toFloat(); //берем значение чувствительности из БД
            PP2=query.value("sensibilityPPwithCd").toFloat(); //берем значение чувствительности из БД
            MPD1=query.value("sensibilityMPDwithoutCd").toFloat(); //берем значение чувствительности из БД
            MPD2=query.value("sensibilityMPDwithCd").toFloat(); //берем значение чувствительности из БД
            MAED1=query.value("sensibilityMAEDwithoutCd").toFloat(); //берем значение чувствительности из БД
            MAED2=query.value("sensibilityMAEDwithCd").toFloat(); //берем значение чувствительности из БД
            //qDebug()<<sensibility;
        }
        db.close();
    }
    switch (mesParam) //выбираем текущую чувствительность в зависимости от выбранных параметра измерения и диапазона
    {
        case 0:
            if(diapParam==0)
            {
                sensCurr=PP1;
            }
            else if(diapParam==1)
            {
                sensCurr=PP2;
            }
            break;
        case 1:
            if(diapParam==0)
            {
                sensCurr=MPD1;
            }
            else if(diapParam==1)
            {
                sensCurr=MPD2;
            }
            break;
        case 2:
            if(diapParam==0)
            {
                sensCurr=MAED1;
            }
            else if(diapParam==1)
            {
                sensCurr=MAED2;
            }
            break;
        default: break;
    }
}

void Block_BDKN::doseRateCount() //функция рассчета окончательного параметра (ПП, МПД, МАЭД)
{
    doseRate=IblockCR_Res/sensCurr;

}

QString Block_BDKN::getBlockName()
{
    return blockName;
}

float Block_BDKN::getDoseRate()
{
    doseRateCount();
    return doseRate;
    //qDebug()<<"doseRate"<<doseRate;
}

quint8 Block_BDKN::getMesParam()
{
    return mesParam;
}

quint8 Block_BDKN::getMesDiap() // метод возвращающий диапазон
{
    return diapParam;
}
