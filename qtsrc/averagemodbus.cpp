//**************************** Класс приема информации из эмулятора или из порта RS-485 по протоколу ModBus RTU ******

#include "averagemodbus.h"

AverageModbus::AverageModbus()
{
    init();
    //qRegisterMetaType<uint8_t>("uint8_t"); //проблемы в Qt  при передаче в сигнале не зарегистрированых по умолчанию типов (строка регистрирует тип. В дальнейшем везде пишем quint8 вместо uint8_t, чтобы не писать эту строку)
}
AverageModbus::~AverageModbus()
{
    if(rs_485!=nullptr)
    {
        rs_485->stop();
    }

    //thread->deleteLater();
    //emit finish();     // посылаем сигнал об окончании работы в потоке для остановки таймера в эмуляторе (чтоб не выкидывало ошибку о том что таймер можно останавливать только из того же потока)
    //thread->quit();  //выход из потока
    //thread->wait();  //ждем непонятно чего {видимо пока закроется поток} (без этой строки при выходе из эмулятора выкидывает со словами:QThread: Destroyed while thread is still running)
    //delete thread;
    qDebug()<<"delete modEmul;";
    delete modEmul;
    delete rs_485;

}
void AverageModbus::init()
{
    file=new QFile("a.txt");
    stream= new QTextStream(file);
    times=1;                           // задаем количество усреднений
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
            //times = static_cast<uint8_t>(query.value("countsAver").toUInt()); // определяем число отсчетов при усреднении из настроек
        }
        //********************************************************************** определяем эмулятор или детектор
        qStr = "SELECT *FROM SourceChoice";
        if(!query.exec(qStr)){qDebug()<<"unable execute query SELECT table SourceChoice"<<query.lastError().text();}
        if(query.first())
        {
            CurrSource = static_cast<bool>(query.value("emulator").toUInt());

        }
        //***********************************************************************

        db.close();
    }
    if(CurrSource==true) //если выбран эмулятор вызываем функцию emul()
    {
        emul();qDebug()<<"Emulator";
    }
    else { rs485();qDebug()<<"Detector RS-485";} //выбран реальный детектор

}

void AverageModbus::emul()
{
    //thread = new QThread;
    modEmul= new Modbus_emulator;
    connect(modEmul,&Modbus_emulator::modReg,this,&AverageModbus::recieveData); //коннект на сигнал из эмулятора
    modEmul->start();

    //
    //connect(thread,&QThread::started,modEmul,&Modbus_emulator::doWork); //коннект на метод doWork при старте потока
    //connect(this,&AverageModbus::finish,modEmul,&Modbus_emulator::endWork);//коннект на метод endWork при выходе из потока
    ////connect(modEmul,&Modbus_emulator::finish,this,&AverageModbus::finishWork);
    //modEmul->moveToThread(thread);   //помещаем объект в поток

    //if(thread->isRunning()) //если поток уже запущен (чтобы не запускать новых)
    //{
    //    qDebug()<<"is running";
    //}
    //else thread->start(); //старт потока
}

void AverageModbus::rs485()
{
    //thread = new QThread;
    rs_485= new RS485;
    connect(rs_485,&RS485::errorSig,this,&AverageModbus::errorConnectionSlot);

    //if(bdg!=nullptr)
    //{
    //    rs_485->bdgINIT(bdg);
    //}
    //if(neutron!=nullptr)
    //{
    //    rs_485->neutronINIT(neutron);
    //}

    connect(rs_485,&RS485::signalData,this,&AverageModbus::recieveData); //коннект на сигнал из эмулятора

    //connect(thread,&QThread::started,rs_485,&RS485::doWork); //коннект на метод doWork при старте потока
    //connect(this,&AverageModbus::finish,rs_485,&RS485::endWork);//коннект на метод endWork при выходе из потока
    //connect(modEmul,&Modbus_emulator::finish,this,&AverageModbus::finishWork);
    //rs_485->moveToThread(thread);   //помещаем объект в поток
    //if(thread->isRunning()) //если поток уже запущен (чтобы не запускать новых)
    //{
    //    qDebug()<<"is running";
    //}
    //else thread->start(); //старт потока
    rs_485->start();
}
void AverageModbus::recieveData(float countRate) //прием сигнала с данными из одного регистра Модбаса (из эмулятора или детектора)
{
    count++;
    avData=avData+countRate; //складываем для дальнейшего усреднения
    if(count==times) // каждые times раз вызываем усреднение
    {
        avData=avData/times; // усредняем показания
        //qDebug()<<avData;
        callAverCR(avData);//вызываем функцию в которой записываем усредненное значение в переменную
        count=0;
        avData=0;
    }
}

float AverageModbus::getAvCR() //возвращаем значение (по запросу из другого класса)
{
    return avCR;
}

void AverageModbus::callAverCR(float avCR) //функция в которой записываем усредненное значение в переменную
{
    this->avCR=avCR;
    /*
    QString str = QString::number(avCR,'f',1);
    if(!file->exists())
    {
        file->open(QIODevice::ReadWrite);
        file->close();
    }
    if(file->exists()&&file->open(QIODevice::Append))
    {
        *stream<<str+"\n";
        file->close();
    }
    */

    //qDebug()<<avCR;
}

void AverageModbus::errorConnectionSlot() //слот приема сигнала от RS-485 и передачи этого сигнала далее
{
    emit errorSig();
}






