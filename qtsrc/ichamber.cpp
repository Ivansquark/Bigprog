//************************ "абстрактный" класс ИК от которого будут наследоваться реальные ИК *********************

#include "ichamber.h"
#include "qapplication.h"

Ichamber::Ichamber(uint8_t r,bool comp,double temp,double p, double corrF):
    iR(r),Comp(comp),Temp(temp),P(p),CorrF(corrF) //конструктор объекта (инициализация параметров происходит в списке инициализации)
{
    init();
}

Ichamber::~Ichamber()
{
    qDebug()<<"~Ichamber()";
    delete timer;
    delete averageADC;
}


void Ichamber::init()
{    
    hC2.resize(2);
    hC2[0]=0x2C; hC2[1]=0x32;
    h90.resize(2);
    h90[0]=0x29; h90[1]=0x30;
    hA0.resize(2);
    hA0[0]=0x2A; hA0[1]=0x30; //инициализируем коды реле управления диапазоном измерения

    averageADC=new AverageADC; //создаем объект класса AverageADC из которого принимаем усредненное значение выдаваемое АЦП (эмулятором или детектором(электрометром))
    connect(this,&Ichamber::writeToComSig,averageADC,&AverageADC::sendToComSlot); //коннект на запись в СОМ порт

    timer = new QTimer(this);
    connect(timer,&QTimer::timeout,this,&Ichamber::timeOut);
    timer->start(500);

    singleTimer = new QTimer(this);
    connect(singleTimer,&QTimer::timeout,this,&Ichamber::singleTimeOut);
    singleTimer->setSingleShot(true);

    //*******************************************************************

    if(QSqlDatabase::contains("myDB"))
    {
        QSqlDatabase db=QSqlDatabase::database("myDB");
        db.setDatabaseName("config.db");
        if(!db.open()){qDebug()<<"db was not opened";}
        QSqlQuery query(db);
        QString qStr = "SELECT *FROM electrometer";
        if(!query.exec(qStr)){qDebug()<<"unable execute query SELECT table electrometer"<<query.lastError().text();}
        if(query.first())
        {
            R1=query.value("R1").toDouble();
        }
         qStr = "SELECT *FROM electrometerTest";
         if(!query.exec(qStr)){qDebug()<<"unable execute query SELECT table electrometerTest"<<query.lastError().text();}
         if(query.first())
         {
             DAC1 = query.value("DAC").toUInt();
             R2=query.value("R2").toDouble();
             R3=query.value("R3").toDouble();
             Iin=query.value("Iin").toLongLong(); //значение компенсации тока утечки электрометра (без подключенной камеры) в разрядах АЦП
         }
         if (Comp==1) //включен флаг компенсации (реализуется потом, пока все Comp равны 0)
         {
             qStr = "SELECT *FROM chamberCompensation";
             if(!query.exec(qStr)){qDebug()<<"unable execute query SELECT table electrometer"<<query.lastError().text();}
             if(query.first())
             {
                Comp1=query.value("Comp1").toInt();
                Comp2=query.value("Comp2").toInt();
                Comp3=query.value("Comp3").toInt();
             }
         } else {CompCurr=0;} // на всякий случай
    }
    bytesDAC.clear();
    QApplication::setOverrideCursor(Qt::WaitCursor);
    switch (iR)  //в зависимости от флага iR выставляем текущее R и Компенсацию и посылаем команду с номером реле в COM порт
    {
    case 0:Rcurr=R3*1e9;CompCurr=Comp1;
        for(long i=0; i<500000000;i++){;}
        emit writeToComSig(hC2);
        bytesDAC.append(hC2);
        break;
    case 1:Rcurr=R2*1e6;CompCurr=Comp2;
        for(long i=0; i<500000000;i++){;}
        emit writeToComSig(h90);
        bytesDAC.append(h90);
        break;
    case 2:Rcurr=R1*1e6;CompCurr=Comp3;
        for(long i=0; i<500000000;i++){;}
        emit writeToComSig(hA0);
        bytesDAC.append(hA0);
        break;
      default:break;
    }
    qDebug()<<"ID1";
    QApplication::restoreOverrideCursor();

    QString strDAC=QString::number(DAC1,16);//переводим в шестнадцатеричный формат
    QString DAC4="",DAC5="",DAC6="";
    DAC4="4"+strDAC[0];
    DAC5="5"+strDAC[1];
    DAC6="6"+strDAC[2];
    //bytesDAC.clear();
    bytesDAC.append(static_cast<char>(DAC4.toUInt(nullptr,16)));
    bytesDAC.append(static_cast<char>(DAC5.toUInt(nullptr,16)));
    bytesDAC.append(static_cast<char>(DAC6.toUInt(nullptr,16)));//записываем вычисленный массив байтов ЦАПа
    // посылаем массив байтов со значениями ЦАПа 4х5х6х для компенсации нуля усилителя
    //emit writeToComSig(bytesDAC);
}

void Ichamber::timeOut() //раз в пол секунды рассчитываем значение Тока (значение АЦП передаются раз в 100мс для эмулятора и не чаще раза в 200 мс (еще и усредняются) для реального электрометра, поэтому расчитывать значение тока чаще не имеет смысла)
{
    account_I();
}


void Ichamber::account_I()
{
    long Ucomp=averageADC->getAverageADC()-Iin-nullADC-CompCurr; //скомпенсированное напряжение (вычитается нулевое значение разряда АЦП, вычитается входное значение АЦП на чувствительном диапазоне соответствующее току утечки, вычитается текущее значение компенсации реального электрометра с подключенной камерой)
    //qDebug()<<Ucomp;

    IchamberRes = ((Ucomp*0.0000003)*CorrF*760*(273+Temp)/(293*P))/Rcurr; //ток в текущих условиях давления и температуры
    //qDebug()<<CorrF<<Temp<<P<<Rcurr;
    // 0.0000003 - коэффициент перевода разряда АЦП в вольты (5В - 16777216 разрядов)
    //qDebug()<<IchamberRes;
}



double Ichamber::getIres() // метод из которого можно узнать текущую силу тока (в заданных значениях температуры и влажности)
{
    //qDebug()<<IchamberRes;
    return IchamberRes;
}

void Ichamber::setVoltageSlot(const QByteArray &volt) //прием из вне значения байтов высокого напряжения, которые необходимо передать далее
{
    bytesDAC.append(volt);
    emit writeToComSig(bytesDAC);
    singleTimer->start(4000);
    //for(long i=0; i<500000000;i++){;}
    //emit writeToComSig(bytesDAC);
    //for(long i=0; i<500000000;i++){;}
    //emit writeToComSig(bytesDAC);
    //for(long i=0; i<500000000;i++){;}
    //emit writeToComSig(bytesDAC);
    //for(long i=0; i<500000000;i++){;}
    //emit writeToComSig(bytesDAC);
}

void Ichamber::singleTimeOut()
{
    emit writeToComSig(bytesDAC);
    sendToElectrometer();
    qDebug()<<bytesDAC.toHex();

    qDebug()<<"Cursor";
}


void Ichamber::sendToElectrometer()
{

    //long x=0;
    //for(long i=0; i<500000000;i++){;}
    emit writeToComSig(bytesDAC);
    //for(long i=0; i<500000000;i++){;}
    //emit writeToComSig(bytesDAC);
    //for(long i=0; i<500000000;i++){;}
    //emit writeToComSig(bytesDAC);
    //for(long i=0; i<500000000;i++){;}
    //emit writeToComSig(bytesDAC);
}
