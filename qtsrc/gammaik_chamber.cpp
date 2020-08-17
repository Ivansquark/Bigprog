#include "gammaik_chamber.h"

GammaIK_chamber::GammaIK_chamber(uint8_t r, bool comp,double temp,double p, double corrF, QString chamName, quint8 mesparam)
    :Ichamber(r,comp,temp,p,corrF),chamberName(chamName),mesParam(mesparam)
{
    init();
}

GammaIK_chamber::~GammaIK_chamber()
{
    qDebug()<<"~GammaIK_chamber()";
}

void GammaIK_chamber::init()
{
    QString str = chamberName;
    int x=str.indexOf(" ");
    chamberName=str.left(x);
    double MAED=0;
    double MNED=0;
    double MIED=0;
    double KERMA=0;
    double MPD=0;
    double MED=0;
    if(QSqlDatabase::contains("myDB"))
    {
        QSqlDatabase db = QSqlDatabase::database("myDB");
        db.setDatabaseName("config.db");
        if(!db.open()){qDebug()<<"db was not opened";}
        QSqlQuery query(db);
        QString qStr="SELECT *FROM gammaChambers WHERE name='%1'";
        QString qStr1=qStr.arg(chamberName);
        if(!query.exec(qStr1)){qDebug()<<"unable execute query SELECT sensibility FROM gammaChambers WHERE name='%1'";}
        if (query.first())
        {
            MAED=query.value("sensibilityMAED").toDouble(); //берем значение чувствительности из БД
            MNED=query.value("sensibilityMNED").toDouble(); //берем значение чувствительности из БД
            MIED=query.value("sensibilityMIED").toDouble(); //берем значение чувствительности из БД
            KERMA=query.value("sensibilityKERMA").toDouble(); //берем значение чувствительности из БД
            MPD=query.value("sensibilityMPD").toDouble(); //берем значение чувствительности из БД
            MED=query.value("sensibilityMED").toDouble(); //берем значение чувствительности из БД
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

void  GammaIK_chamber::doseRateCount() //метод из которого можно узнать мощность дозы
{
    doseRate = std::abs(getIres()*sensCurr);
    //qDebug()<<getIres()<<sensCurr<<doseRate;
    //return doseRate;       //функция из предка Ichamber (возвращает значение тока из Ichamber, умноженного на коэф чувствительности)
}

double GammaIK_chamber::getDoseRate()
{
    doseRateCount();
    return doseRate;
}

QString GammaIK_chamber::getBlockName()
{
    return chamberName;
}

quint8 GammaIK_chamber::getMesParam()
{
    return mesParam;
}
