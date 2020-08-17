//*************************** Класс COM порта (RS-232) ****************************
//*********************************************************************************
//******** Открываем порт, после паузы в 6 сек, по таймеру раз в 100 мс производим считывание массива байтов  *********
//******** из порта, сортируем их (чтобы было значение типа 2f3f4f5f6f7f), делаем из массива строку и отправляем ее далее.  ******
//******** Также записываем информацию в порт (в ) **********

#include "rs232.h"

RS232::RS232(QObject* parent):QObject(parent)
{
    init();
}
RS232::~RS232()
{
    delete timer;
    delete startTimer;
    delete comPort;
}

void RS232::init()
{
    comPort = new QSerialPort(this);
    comPort->setPortName("/dev/ttyS0");
    comPort->setBaudRate(QSerialPort::Baud2400); //на такой скорости работает электрометр
    comPort->setDataBits(QSerialPort::Data8);
    comPort->setParity(QSerialPort::NoParity);
    comPort->setStopBits(QSerialPort::OneStop);
    comPort->setFlowControl(QSerialPort::NoFlowControl);

    timer = new QTimer(this);
    connect(timer,&QTimer::timeout,this,&RS232::timeOut);
    startTimer = new QTimer(this);
    connect(startTimer,&QTimer::timeout,this,&RS232::startTimeOut);
    connectPort();
}

void RS232::connectPort()
{
    if(comPort->open(QIODevice::ReadWrite))
    {
        if(!comPort->isOpen())
        {
            qDebug()<<"error occured while opening port";
            comPort->close();
        }
        else
        {
            qDebug()<<"Port RS-232 is connected";
            comPort->setDataTerminalReady(true);
            comPort->setRequestToSend(true);
        }
    }
    startTimer->setSingleShot(true);  //устанавливаем срабатывание таймера только один раз
    startTimer->start(5000);          //пауза перед началом измерения с порта RS-232, т.к. данные начинают идти не сразу
}

void RS232::disconnectPort()
{
    timer->stop();
    if(comPort->isOpen())
    {
        qDebug()<<"disconnect";
        comPort->close();
    }
    else
    {
        comPort->close();
    }
}

void RS232::startTimeOut() //вызывается при срабатывании стартового таймера
{
    timer->start(100);
    qDebug()<<"start reading port RS-232";
}

void RS232::timeOut() //ф-я чтения с порта RS-232 каждые 100 мс (за это время должна заполнится очередь из байтов)
{
    if(comPort->isOpen())
    {
        long long i = comPort->QSerialPort::bytesAvailable(); //значение байт доступных для чтения (находящихся в очереди)
        QByteArray arrList; //переменная создана на стэке => сама очищается при выходе из функции
        if(i>=7)                                //если число доступных байт больше 7 (с АЦП - идет повторяющаяся последовательность из 6 байт)
        {
            arrList.append((comPort->read(6))); //добавляем в массив байт шесть принятых байт (могут прийти в любом порядке, затем отсортируем их)

             qint8 *arr = new qint8 [6]; //вспомогательный массив
             for(int i=0;i<6;i++)
             {
                 arr[i]=arrList[i];
             }
             std::sort(&arr[0],&arr[6]); // функция сортировки байтов по порядку (при высокой скорости считывания иногда теряется порядок!!! и пропускаются байты)

             if(arr[0]!=arr[1]&&arr[0]!=arr[2]&&arr[0]!=arr[3]&&arr[0]!=arr[4]&&arr[0]!=arr[5]
                     &&arr[1]!=arr[2]&&arr[1]!=arr[2]&&arr[1]!=arr[3]&&arr[1]!=arr[4]&&arr[1]!=arr[5]
                     &&arr[2]!=arr[3]&&arr[2]!=arr[4]&&arr[2]!=arr[5]
                     &&arr[3]!=arr[4]&&arr[3]!=arr[5]
                     &&arr[4]!=arr[5]) // условие неравенства байтов между собой (иногда пропускались байты и было несколько одинаковых!)
             {
                 COM=QString::number(arr[0],16)+QString::number(arr[1],16)+
                     QString::number(arr[2],16)+QString::number(arr[3],16)+
                     QString::number(arr[4],16)+QString::number(arr[5],16); //делаем строку из упорядоченного массива байтов;
             }
             delete [] arr; //очищаем память
        }
       emit outRS232(COM); //посылаем сигнал со строкой в которой зашифрованы данные АЦП
       //qDebug()<<COM;
       comPort->clear(); // очищаем очередь порта
    }
}

void RS232::writeToRS232(QByteArray& data) //ф-я записи в порт RS-232
{
    if(comPort->isOpen())
    {
        timer->stop(); // останавливаем таймер по которому производится считывание (если не останавливать почему то не записывается в Linux-е, в Windows все норм - ЗАГАДКА!)
        //comPort->clear();//- очищаем порт
        comPort->write(data);                       //записываем данные
        //QByteArray x=data;
        //for(auto i:x)
        //{
        //    qDebug()<<QString::number(i,16);
        //}

        qDebug()<<"WriteTo Port!!!!==== "<<data.toHex();

        //qDebug()<<QString::number(data[0],16)<<
        //          QString::number(data[1],16)<<
        //          QString::number(data[2],16)<<
        //          QString::number(data[3],16)<<
        //          QString::number(data[4],16)<<
        //          QString::number(data[5],16)<<
        //          QString::number(data[6],16);
        //comPort->clear();//- очищаем порт
        timer->start(100); //заново запускаем процесс чтения
    }
}



