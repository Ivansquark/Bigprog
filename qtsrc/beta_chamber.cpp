#include "beta_chamber.h"

Beta_chamber::Beta_chamber(uint8_t r, bool comp,double temp,double p, double corrF, QString chamName)
    :Ichamber(r,comp,temp,p,corrF),chamberName(chamName)
{
    init();
}

Beta_chamber::~Beta_chamber()
{    
    qDebug()<<"~Beta_chamber()";
}

void Beta_chamber::init()
{
    QString str = chamberName;
    int x=str.indexOf(" ");
    chamberName=str.left(x);
    if(QSqlDatabase::contains("myDB"))  //из БД из таблицы с Бета камерами узнаем чувствительность
    {
        QSqlDatabase db=QSqlDatabase::database("myDB");
        db.setDatabaseName("config.db");
        if(!db.open()){qDebug()<<"db was not opened";}
        QSqlQuery query(db);
        QString qStr="SELECT *FROM betaChambers WHERE name='%1'";
        QString qStr1=qStr.arg(chamberName);
        if(!query.exec(qStr1)){qDebug()<<"unable execute query SELECT sensibility FROM betaChambers WHERE name='%1'";}
        if (query.first())
        {
            sensibility=query.value("sensibility").toDouble(); //берем значение чувствительности из БД
            //qDebug()<<sensibility;
        }
        db.close();
    }    
}

double Beta_chamber::MPD()
{
    return std::abs(getIres()*sensibility);       //функция из предка Ichamber (возвращает значение тока из Ichamber, умноженного на коэф чувствительности)
    //qDebug()<<getIres()*sensibility;
}

QString Beta_chamber::getChamberName()
{
    return chamberName;
}
