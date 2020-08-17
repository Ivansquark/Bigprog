#include "rs485.h"
//#include "neutron_measuring.h"
//#include "gamma_bdg_measuring.h"

bool RS485::BDGorBDKN=0;

RS485::RS485(QObject* parent):QObject(parent)
{
    init();
}

RS485::~RS485()
{
    if(modbusDevice)
    {
        modbusDevice->disconnectDevice();
    }
    delete modbusDevice;
    delete timer;
    delete msg;
}

void RS485::init()
{    
    if(!BDGorBDKN)
    {
        SERVER_ADDRESS = 0x08;
        START_ADDRESS = 0x10d;
        AMOUNT = 1;
        qDebug()<<"Address of BDG";
    }
    else
    {
        SERVER_ADDRESS = 1;
        START_ADDRESS = 0x10d; // если второй канал (control)
        //START_ADDRESS = 0xdd; // если первый канал (measure)

        AMOUNT = 1;
        qDebug()<<"Address of BDKN";
    }
    modbusDevice = new QModbusRtuSerialMaster(this); // создаем объект как мастер

    //connect(modbusDevice, &QModbusClient::errorOccurred, [this](QModbusDevice::Error){statusBar()->showMessage(modbusDevice->errorString(), 5000);});
    //ЛЯМБДА функция (локальная, где вызывается там и выполняется)  не будем ей пользоваться
    connect(modbusDevice,&QModbusClient::errorOccurred,this,&RS485::error); //сигнал по ошибке

    if(modbusDevice) //если создался объект
    {
        //connect(modbusDevice, &QModbusClient::stateChanged, this, &MainWindow::onStateChanged);
        connect(modbusDevice,&QModbusClient::stateChanged,this,&RS485::onStateChanged); //сигнал по изменению статуса
    }
    timer=new QTimer(this);
    connect(timer,&QTimer::timeout,this,&RS485::timeOut); //коннектим таймер
    ConnectPort();    
}

void RS485::start()
{
    timer->start(500);
    modbusDevice->setTimeout(100);
    //ConnectPort();
}

void RS485::stop()
{
    timer->stop();
    DisconnectPort();
}

QModbusDataUnit RS485::readRequest() const  //запрос на чтение holding_registers (функция 03)
{
    qDebug()<<"START_ADDRESS "<<START_ADDRESS;
    return QModbusDataUnit(QModbusDataUnit::HoldingRegisters, START_ADDRESS, static_cast<quint16>(AMOUNT));
}

QModbusDataUnit RS485::writeRequest() const  //запрос на запись holding_register (функция 06) пока не нужна
{
    return QModbusDataUnit(QModbusDataUnit::HoldingRegisters, START_ADDRESS,static_cast<quint16>(WRITE_SIZE));
}



void RS485::ConnectPort()
{
    qDebug()<<"ConnectPort";

    if (!modbusDevice) {return;}  //если нету указателя на объект то выход из метода

    if (modbusDevice->state() != QModbusDevice::ConnectedState)  //если не соединено устанавливаем соединение с параметрами:
    {
        modbusDevice->setConnectionParameter(QModbusDevice::SerialPortNameParameter, PORT);
        modbusDevice->setConnectionParameter(QModbusDevice::SerialParityParameter, PARITY);
        modbusDevice->setConnectionParameter(QModbusDevice::SerialBaudRateParameter, BAUDS);
        modbusDevice->setConnectionParameter(QModbusDevice::SerialDataBitsParameter, DATA_BITS);
        modbusDevice->setConnectionParameter(QModbusDevice::SerialStopBitsParameter, STOP_BITS);
        modbusDevice->setTimeout(RESPONSE_TIME);
        qDebug()<<PORT<<PARITY<<BAUDS<<DATA_BITS<<STOP_BITS;
        modbusDevice->setNumberOfRetries(0);        //повторные попытки соединения
        if (!modbusDevice->connectDevice())   // коннектимся (если не получилось выдаем ошибку с номером)
        {
            qDebug()<<modbusDevice->errorString();
        }
    }
    else                                        // если соединено, то разрываем соединение
    {
        modbusDevice->disconnectDevice();
    }

    //timer->start(500); //считываем каждые полсекунды
}

void RS485::DisconnectPort()
{    
    //timer->stop();  //останавливаем таймер, чтобы остановить запросы по RS-485
    modbusDevice->disconnectDevice();
}

void RS485::ReadPort() //метод считывания из порта
{
    if (!modbusDevice)
    {return;}
    qDebug()<<"SERVER_ADDRESS"<<SERVER_ADDRESS;
    if (QModbusReply *lastRequest = modbusDevice->sendReadRequest(readRequest(), SERVER_ADDRESS)) //посылаем запрос чтения по таймеру
        //функция от наследника QModbusDevice:QModbusClient
        // если  существует lastRequest (ответ от модбаса по запросу чтения)
    {
        if (!lastRequest->isFinished())   //и если булевая функция isFinished==0
        {
            connect(lastRequest,&QModbusReply::finished,this,&RS485::readyRead); //создаем коннект на сигнал QModbusReply::finished о готовности к чтению
            //delete lastRequest;
        }
        else   delete lastRequest; // broadcast replies return immediately (широковещательный запрос возвращается немедленно)
    }
    else //(lastRequest==nullptr)
    {
       qDebug()<<"Read error: " + modbusDevice->errorString(); // если нет ответа от модбаса выводим ошибку
    }
}

void RS485::readyRead() //сигнал в ответ на запрос чтения по таймеру
{
    //QModbusReply* lastRequest;
    auto *lastRequest=static_cast<QModbusReply *>(sender()); //object_cast<> ~ static_cast
    if (!lastRequest)
    {
        return;
    }

    if (lastRequest->error() == QModbusDevice::NoError)
    {
        //***********************************************************************************************************
        const QModbusDataUnit unit = lastRequest->result(); //записываем значения выдаваемое модбасом по запросу в класс контейнера
        quint16 f1=0;//,f2=0,f3=0,f4=0, h1=0;
        //qfloat16 hf=0;
        uint32_t x=0;
        float z1=0;
        //float z2=0;
        //float z3=0;
        //float z4=0;
        f1=static_cast<quint16>(unit.value(0));//для эмулятора на STM32
        //f4=static_cast<quint16>(unit.value(15));//для эмулятора на STM32
        //qDebug()<<f1<<f2<<f3<<f4;
        //x=(f1<<16)|0x0;
        //z1=*reinterpret_cast<float*>(&x)*3600;
        //x=(f2<<16)|0x0;
        //z2=*reinterpret_cast<float*>(&x)*3600;
        x=(f1<<16)|0x0;
        z1=*reinterpret_cast<float*>(&x);
        //x=(f4<<16)|0x0;
        //z4=*reinterpret_cast<float*>(&x);
        //qDebug()<<z1<<z2<<z3<<z4;
        qDebug()<<unit.values();

        //h1=static_cast<quint16>(unit.value(3));//для эмулятора на STM32

        //qDebug()<<QString::number(f1,2)<<QString::number(f2,2)<<QString::number(h1,2);

        //void* h1_ptr = (void*)(&h1);
        //hf=*((qfloat16*)(h1_ptr));

        //x=static_cast<uint32_t>(f1<<16|f2); //объединяем два 16 битных регистра в один 32 битный.

        //void* v = static_cast< void* >( &x ); //сначала приводим к безтиповому указателю на void* (скинем тип указателя, чтобы принудительно не переводил int во float)
        //z = *static_cast< float* >( v ); // затем разыменовываем указатель на float* предварительно приведенный из типа указателя на float*

        emit signalData(z1); //посылаем скорость счета в формате float (В ТОМ ЧИСЛЕ ДЛЯ ТОРОПЫ)

        //emit signalData(z=hf); //посылаем скорость счета в формате float (предварительно переведя из half float)
        //qDebug()<<"float= "<<z<<" half float= "<<hf;

        //float xF = *reinterpret_cast<float*>(&v); //тоже самое но в одну строку, только получаем указатель переводим в float
        //qDebug()<<QString::number(y1,2)<<QString::number(y2,2);
        // qDebug()<<QString::number(x,2);

        for (uint i = 0; i < unit.valueCount(); i++)
        {
            const QString entry = tr("Address: %1, Value: %2").arg(unit.startAddress()+static_cast<int>(i))
                                     .arg(QString::number(unit.value(static_cast<int>(i))));
            //ui->readValue->addItem(entry); // добавляем значения в таблицу, которая выводится на экран
        }
        //float z=static_cast<float>(x); // так превращает int сразу во float (просто делает большое число, что есть полный бред)

        //************************************************************************************************************
    }
    else if (lastRequest->error() == QModbusDevice::ProtocolError)
    {
        QString str="Read response error: %1 (Mobus exception: 0x%2)";
        QString str2=str.arg(lastRequest->errorString()).arg(lastRequest->rawResult().exceptionCode());
        qDebug()<<str2<<" Protocol error";
    }
    else
    {
        QString str="Read response error: %1 (code: 0x%2)";
        QString str2=str.arg(lastRequest->errorString()).arg(lastRequest->rawResult().exceptionCode());
        qDebug()<<str2<<" Just error";
        qDebug()<<"Read error: " + modbusDevice->errorString();
        qDebug()<<lastRequest->error();
        qDebug()<<static_cast<quint16>(lastRequest->result().value(0));
        error();
    }
    lastRequest->deleteLater();
}

void RS485::WriteToRegister()
{
    //START_ADDRESS=;  //записываем в переменную адрес регистра для записи
    if (!modbusDevice)
    {return;}

    QModbusDataUnit writeUnit = writeRequest(); // объявляем объект класса контейнера QModbusDataUnit и выполняем запрос на запись
    //QModbusDataUnit::RegisterType table = writeUnit.registerType();
    //writeUnit.setValue(0, static_cast<quint16>(1));  //установим значение по указанному адресу

    if (QModbusReply *lastRequest = modbusDevice->sendWriteRequest(writeUnit, SERVER_ADDRESS)) //если существует такой запрос
    {
        if (!lastRequest->isFinished())
        {
            qDebug()<<writeUnit.value(0);
            if (lastRequest->error() == QModbusDevice::ProtocolError)
            {
                QString str="Write response error: %1 (Mobus exception: 0x%2)";
                QString str2=str.arg(lastRequest->errorString()).arg(lastRequest->rawResult().exceptionCode());
                qDebug()<<str2;
            }
            else if (lastRequest->error() != QModbusDevice::NoError)
            {
                QString str="Write response error: %1 (code: 0x%2)";
                QString str2=str.arg(lastRequest->errorString()).arg(lastRequest->rawResult().exceptionCode());
                qDebug()<<str2;
            }
        }
        else
            {
                // broadcast replies return immediately
                lastRequest->deleteLater();
            }
       //lastRequest->deleteLater();
    }
    else //если запрос не удался
    {
            qDebug()<<"Write error: " + modbusDevice->errorString();            
    }
}

void RS485::timeOut()
{
    ReadPort();
}

void RS485::error() //ошибка соединения
{
    timer->stop();
    //if(bdgPtr!=nullptr)
    //{
    //    //bdgPtr->
    //}
    //if(neutronPtr!=nullptr)
    //{
    //    //callback
    //}
    //
    //if(msg==nullptr)
    //{
    //    //emit error();
    //    //QMessageBox::warning(this, "Ошибка","Некорректный ввод");
    //
    //    //msg = new QMessageBox(QMessageBox::Information,"ERROR","Error",QMessageBox::Ok);
    //    //msg->setModal(false);
    //    //msg->show();
    //}
    //Gamma_BDG_measuring::errorConnection();
    qDebug()<<"ERROR connection";
    emit errorSig();
}


void RS485::onStateChanged(quint8 state) //статус соединения
{
    if (state == QModbusDevice::UnconnectedState)
    {qDebug()<<"disonnected";}
    else if (state == QModbusDevice::ConnectedState)
    {qDebug()<<"Connected";}
}

void RS485::doWork()
{
    start();
}

void RS485::endWork()
{
    stop();
}



