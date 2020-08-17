#include "block_bdg.h"

Block_BDG::Block_BDG(float temp,float p, float corrF, QString name, quint8 mesparam):
    Iblock(temp, p, corrF),blockName(name),mesParam(mesparam)
{
    init();
}
Block_BDG::~Block_BDG()
{
    //qDebug()<<"~Block_BDG()";
}

void Block_BDG::init()
{
    QString str = blockName;
    int x=str.indexOf(" ");
    blockName=str.left(x);
    float MAED=0;
    float MNED=0;
    float MIED=0;
    float KERMA=0;
    float MPD=0;
    float MED=0;
    if(QSqlDatabase::contains("myDB"))
    {
        QSqlDatabase db = QSqlDatabase::database("myDB");
        db.setDatabaseName("config.db");
        if(!db.open()){qDebug()<<"db was not opened";}
        QSqlQuery query(db);
        QString qStr="SELECT *FROM gammaBlock WHERE nuclide='%1'";
        QString qStr1=qStr.arg(blockName);
        if(!query.exec(qStr1)){qDebug()<<"unable execute query SELECT sensibility FROM gammaBlock WHERE nuclide='%1'";}
        if (query.first())
        {
            MAED=query.value("sensibilityMAED").toFloat(); //берем значение чувствительности из БД
            MNED=query.value("sensibilityMNED").toFloat(); //берем значение чувствительности из БД
            MIED=query.value("sensibilityMIED").toFloat(); //берем значение чувствительности из БД
            KERMA=query.value("sensibilityKERMA").toFloat(); //берем значение чувствительности из БД
            MPD=query.value("sensibilityMPD").toFloat(); //берем значение чувствительности из БД
            MED=query.value("sensibilityMED").toFloat(); //берем значение чувствительности из БД
            //qDebug()<<sensibility;
        }
        db.close();
    }
    switch (mesParam) //выбираем текущую чувствительность в зависимости от выбранного параметра измерения
    {
        case 0: sensCurr=MAED; break;
        case 1: sensCurr=MNED; break;
        case 2: sensCurr=MIED; break;
        case 3: sensCurr=KERMA; break;
        case 4: sensCurr=MPD; break;
        case 5: sensCurr=MED; break;
        default: break;
    }
}

void Block_BDG::doseRateCount()
{
    doseRate=IblockCR_Res/sensCurr;
}

QString Block_BDG::getBlockName()
{
    return blockName;
}

float Block_BDG::getDoseRate()
{
    doseRateCount();
    return doseRate;
}

quint8 Block_BDG::getMesParam()
{
    return mesParam;
}
