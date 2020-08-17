#include "electrometer_test.h"
#include "ui_electrometer_test.h"

Electrometer_test::Electrometer_test(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Electrometer_test)
{
    ui->setupUi(this);
    init();

}

Electrometer_test::~Electrometer_test()
{
    //qDebug()<<"delete avADC";
    delete avADC;
    //qDebug()<<"delete msg";
    delete msg;
    //qDebug()<<"delete timer";
    delete timer;
    //qDebug()<<"delete timerGetADC";
    delete timerGetADC;
    //qDebug()<<"delete elTestRes";
    delete elTestRes;
    //qDebug()<<"delete ui";
    delete ui;
    //qDebug()<<"delete ui";
}

void Electrometer_test::testSource(bool test)
{
    test_source=test;
}


void Electrometer_test::init()
{    
    setWindowTitle("Тестирование электрометра");
    h00.resize(2);
    h00[0]=0x20; h00[1]=0x30;
    h21.resize(2);
    h21[0]=0x22; h21[1]=0x31;
    h29.resize(2);
    h29[0]=0x22; h29[1]=0x39;
    h15.resize(2);
    h15[0]=0x21; h15[1]=0x35;
    h03.resize(2);
    h03[0]=0x20; h03[1]=0x33;
    hC2.resize(2);
    hC2[0]=0x2C; hC2[1]=0x32;
    h82.resize(2);
    h82[0]=0x28; h82[1]=0x32;
    h90.resize(2);
    h90[0]=0x29; h90[1]=0x30;
    hA0.resize(2);
    hA0[0]=0x2A; hA0[1]=0x30;

    setAttribute(Qt::WA_DeleteOnClose);
    //проверка на эмулятор
    bool CurrSource=false;
    if(QSqlDatabase::contains("myDB"))
    {
        QSqlDatabase db = QSqlDatabase::database("myDB");
        db.setDatabaseName("config.db");
        QSqlQuery query(db);
        QString qStr = "SELECT *FROM SourceChoice";
        if(!query.exec(qStr)){qDebug()<<"unable execute query SELECT table SourceChoice"<<query.lastError().text();}
        if(query.first())
        {
            CurrSource = static_cast<bool>(query.value("emulator").toUInt());
        }
        //***********************************************************************
        qStr = "SELECT * FROM electrometer";
        if(!query.exec(qStr)){qDebug()<<"unable execute query SELECT table electrometer"<<query.lastError().text();}
        if(query.first())
        {
            heatingTime = quint8(query.value("heatingTime").toInt());
            timeoutTest = quint8(query.value("testTimeout").toInt());
            R1 = query.value("R1").toInt();
            //averCounts = query.value("countsAver").toInt();
            averCounts=5;
        }
        db.close();
    }
    if(CurrSource==true) //если выбран эмулятор вызываем функцию emul()
    {
        qDebug()<<"Emulator";
        close();                 //если был выбран эмулятор закрываем это окно
    }
    else { qDebug()<<"Включаем тест";} //выбран реальный детектор
    avADC = new AverageADC();
    connect(this,&Electrometer_test::sendToCom,avADC,&AverageADC::sendToComSlot);
    timerGetADC = new QTimer(this);
    connect(timerGetADC,&QTimer::timeout,this,&Electrometer_test::SlotGetADC);
    timerGetADC->start(500);
    timer = new QTimer(this);
    connect(timer,&QTimer::timeout,this,&Electrometer_test::timeOut);
    timer->start(1000);
    //averageADC=avADC->getAverageADC(); //0xffffff
    ui->progressBar->setValue(0); //бегущую строку установим в ноль
}

void Electrometer_test::delay(uint x)
{
    for(uint i=0;i<x;i++);
    //while(x--);
}



void Electrometer_test::timeOut() //ф-я измерения параметров электрометра
{
    time++; //увеличиваем время каждую секунду
    ui->label_4->setText(QString::number(time));
    comCheck+=std::abs(averageADC);
    //********************************************************** Основная функция **********************************
    if(time>=7&&time<9)  //проверка на корректное соединение!!!!!!!!!
    {
        qDebug()<<comCheck;
        if(comCheck<=10||QString::number(comCheck,'f',1)=="nan") //нет инфы с детектора (или около 0 или nan)
        {
            time=0;
            msg=new QMessageBox(QMessageBox::Critical,"ОШИБКА","Нет информации с детектора, \n необходим ремонт!!!");
            msg->setStandardButtons(QMessageBox::Ok);
            msg->setButtonText(QMessageBox::Ok,"ОК");
            //msg->setStyleSheet("QLabel{min-height: 350px;min-width:150 px;font-size:35px;}"
            //                  "QPushButton{height:200px; width:1040px; font-size: 50px;}");
            msg->show();
            timer->stop();// остановим таймер чтобы не считал далее
            if(msg->exec()==QMessageBox::Ok)
            {
                //emit closePortTest();
                //close();
                emit closeTestAll();
                close();
            }
        }
    }
//*******************************************  Прогрев (прибор должен прийти в себя после вкючения [уйти накопившийся заряд и т.д])
    if(time<heatingTime)
    {
        ui->label_2->setText("Прогрев");
    }
    if (time==heatingTime)
    {
        emit sendToCom(h21);
        delay(10000);
        emit sendToCom(h21);
    }
//*************************************** Выставление кода ЦАП на усилителе, для нулевого входного смещения усилителя ***************
    if (time==heatingTime+timeoutTest)
    {
        //emit sendToCom(h00);
        //delay(10000);
        emit sendToCom(h00);
        ui->label_2->setText("Самотестирование");
        ui->progressBar->setValue(0);
    }
    if (time==heatingTime+2*timeoutTest)
    {
        ui->label_2->setText("Определение напряжения смещения");
        emit sendToCom(h21);
        delay(10000);
        emit sendToCom(h21);
        ui->progressBar->setValue(0);
    }
    if (time==heatingTime+3*timeoutTest)
    {
        bytesDAC.clear();
        bytesDAC.append(0x40);
        bytesDAC.append(0x50);
        bytesDAC.append(0x60);
        emit sendToCom(bytesDAC);
        delay(10000);
        emit sendToCom(bytesDAC);
        ui->progressBar->setValue(10);
    }
    if (time>=heatingTime+4*timeoutTest && time<=heatingTime+4*timeoutTest+averCounts)
    {
        ui->progressBar->setValue(20+5*(time-(heatingTime+4*timeoutTest)));
        y+=averageADC;
        //qDebug()<<y;
        if(time==heatingTime+4*timeoutTest+averCounts)
        {
            l1=y/(averCounts+1); // зреднее значение АЦП в разрядах
            qDebug()<<"l1"<<l1;
            y=0;
        }
       // l1=QString::number(averADC_V).toLong();
    }
    if (time==heatingTime+5*timeoutTest+averCounts)
    {
        ui->progressBar->setValue(50);
        bytesDAC.clear();
        bytesDAC.append(0x4f);
        bytesDAC.append(0x5f);
        bytesDAC.append(0x6f);
        emit sendToCom(bytesDAC);
        delay(10000);
        emit sendToCom(bytesDAC);
    }
    if (time>=heatingTime+6*timeoutTest+averCounts && time<=heatingTime+6*timeoutTest+2*averCounts)
    {
        ui->progressBar->setValue(50+5*(time-(heatingTime+6*timeoutTest+averCounts)));
        y+=averageADC;
        if(time==heatingTime+6*timeoutTest+2*averCounts)
        {
            l2=y/(averCounts+1); // зреднее значение АЦП в разрядах
            qDebug()<<"l2"<<l2;
            y=0;
        }

        //l2=QString::number(averADC_V).toLong();
    }
    if (time==heatingTime+6*timeoutTest+2*averCounts)
    {
        ui->progressBar->setValue(80);
        qDebug()<<ADC0<<l1<<l2;
        d=std::abs((static_cast<double>(ADC0)-static_cast<double>(l1))/(static_cast<double>(l2)-static_cast<double>(l1)));
        qDebug()<<d;
        double DAC=0;
        DAC=0xFFF*d;
        qDebug()<<DAC;
        DAC1=static_cast<uint>(DAC);        //окончательное значение ЦАП - необходимо записать в БД
        QString strDAC=QString::number(DAC1,16);//переводим в шестнадцатеричный формат
        QString DAC4="",DAC5="",DAC6="";
        DAC4="4"+strDAC[0];
        DAC5="5"+strDAC[1];
        DAC6="6"+strDAC[2];
        bytesDAC.clear();
        bytesDAC.append(static_cast<char>(DAC4.toUInt(nullptr,16)));
        bytesDAC.append(static_cast<char>(DAC5.toUInt(nullptr,16)));
        bytesDAC.append(static_cast<char>(DAC6.toUInt(nullptr,16)));//записываем вычисленный массив байтов ЦАПа
    }
    if (time==heatingTime+6*timeoutTest+2*averCounts)
    {
        emit sendToCom(bytesDAC);
        delay(10000);
        emit sendToCom(bytesDAC);
    }
    if (time>=heatingTime+7*timeoutTest+2*averCounts && time<=heatingTime+7*timeoutTest+3*averCounts)
    {
        y+=averageADC;
        qDebug()<<y;
        if(time==heatingTime+7*timeoutTest+3*averCounts)
        {
            ADC_offset=(y-ADC0)*0.0000003/(averCounts+1); // cреднее значение АЦП в разрядах
            y=0;            
        }
    }
    //***************** Калибровка R2 **********************************************************************************
    if (time==heatingTime+7*timeoutTest+3*averCounts)
    {
        ui->progressBar->setValue(0);
        ui->label_2->setText("Калибровка R2");
        emit sendToCom(h29);
        delay(10000);
        emit sendToCom(h29);
    }
    if (time==heatingTime+8*timeoutTest+3*averCounts)
    {
        ui->progressBar->setValue(20);
    }
    if (time>=heatingTime+8*timeoutTest+3*averCounts && time<=heatingTime+8*timeoutTest+4*averCounts)
    {
        ui->progressBar->setValue(20+10*(time-(heatingTime+8*timeoutTest+3*averCounts)));
        y+=averageADC;
        if(time==heatingTime+8*timeoutTest+4*averCounts)
        {
            l1=y/(averCounts+1); // зреднее значение АЦП в разрядах
            R2=((ADC0*R1*1000000)/(l1-ADC0) - 100000)/1000000;   // - калибровка R2, в МОм
            qDebug()<<"R2"<<R2;
            y=0;
        }
    }
    //****************************** Калибровка R3 ***************************************************************
    if (time==heatingTime+8*timeoutTest+4*averCounts)
    {
        ui->progressBar->setValue(0);
        ui->label_2->setText("Калибровка R3");
        emit sendToCom(h15);
        delay(10000);
        emit sendToCom(h15);
    }
    if (time==heatingTime+9*timeoutTest+4*averCounts)
    {
        ui->progressBar->setValue(20);
    }
    if (time>=heatingTime+9*timeoutTest+4*averCounts && time<=heatingTime+9*timeoutTest+5*averCounts)
    {
        ui->progressBar->setValue(20+10*(time-(heatingTime+9*timeoutTest+4*averCounts)));
        y+=averageADC;
        if(time==heatingTime+9*timeoutTest+5*averCounts)
        {
            l1=y/(averCounts+1); // зреднее значение АЦП в разрядах
            R3=((ADC0*R2*1000000)/(l1-ADC0) - 100000)/1000000000; // - калибровка R3, в ГОм
            qDebug()<<"R3"<<R3;
            y=0;            
        }
    }
//*************************************** Измерение тока ********************************************

    if (time==heatingTime+9*timeoutTest+5*averCounts)
    {
        ui->progressBar->setValue(0);
        ui->label_2->setText("Входной ток");
        emit sendToCom(h03);
        delay(10000);
        emit sendToCom(h03);// К1+К9
    }
    if (time==heatingTime+10*timeoutTest+5*averCounts)
    {
        ui->progressBar->setValue(5);
        ui->label_2->setText("Входной ток");
    }
    if (time==heatingTime+11*timeoutTest+5*averCounts)
    {
        ui->progressBar->setValue(10);
    }
    if (time==heatingTime+12*timeoutTest+5*averCounts)
    {
        ui->progressBar->setValue(15);
    }
    if (time==heatingTime+13*timeoutTest+5*averCounts)
    {
        ui->progressBar->setValue(20);
    }
    if (time==heatingTime+14*timeoutTest+5*averCounts)
    {
        ui->progressBar->setValue(25);
    }
    if (time==heatingTime+15*timeoutTest+5*averCounts)
    {
        ui->progressBar->setValue(30);
    }
    if (time>=heatingTime+15*timeoutTest+5*averCounts && time<=heatingTime+53*timeoutTest+6*averCounts)
    {
        ui->progressBar->setValue(40+10*(time-(heatingTime+53*timeoutTest+5*averCounts)));
        y+=averageADC;
        if(time==heatingTime+15*timeoutTest+6*averCounts)
        {
            l1=y/(averCounts+1); // зреднее значение АЦП в разрядах
            U_Ioff=(l1-ADC0);
            qDebug()<<"U_Ioff= "<<U_Ioff;
            I=(U_Ioff*0.0000003*1e-9)/R3;
            qDebug()<<"I"<<I;
            y=0;
            if(QSqlDatabase::contains("myDB"))
            {
                QSqlDatabase db=QSqlDatabase::database("myDB");
                db.setDatabaseName("config.db");
                if(!db.open()){qDebug()<<"db was not opened";}
                QSqlQuery query(db);
                //************************************************
                QString qStr="UPDATE electrometerTest SET DAC='%1'";
                QString qStr1 = qStr.arg(QString::number(DAC1));
                if(!query.exec(qStr1)){qDebug()<<"unable execute query update electrometerTest"<<query.lastError().text();}
                //************************************************
                qStr="UPDATE electrometerTest SET Uoff='%1'";
                qStr1 = qStr.arg(QString::number(ADC_offset));
                if(!query.exec(qStr1)){qDebug()<<"unable execute query update electrometerTest"<<query.lastError().text();}
                //************************************************
                qStr="UPDATE electrometerTest SET R2='%1'";
                qStr1 = qStr.arg(QString::number(R2));
                if(!query.exec(qStr1)){qDebug()<<"unable execute query update electrometerTest"<<query.lastError().text();}

                //************************************************
                qStr="UPDATE electrometerTest SET R3='%1'";
                qStr1 = qStr.arg(QString::number(R3));
                if(!query.exec(qStr1)){qDebug()<<"unable execute query update electrometerTest"<<query.lastError().text();}

                qStr="UPDATE electrometerTest SET Iin='%1'";
                qStr1 = qStr.arg(QString::number(U_Ioff));
                if(!query.exec(qStr1)){qDebug()<<"unable execute query update electrometerTest"<<query.lastError().text();}
                db.close();
            }
            ui->progressBar->setValue(100);
            //emit testEnd();

            elTestRes=new Electrometer_test_result(this);
            elTestRes->setModal(true);
            elTestRes->show();
            connect(elTestRes,&Electrometer_test_result::onCloseSig,this,&Electrometer_test::onCloseSlot);
            connect(elTestRes,&Electrometer_test_result::testOkSig,this,&Electrometer_test::testOkSlot);
            //close();
            //delete this;
        }
    }
}

void Electrometer_test::SlotGetADC()
{
    averageADC=avADC->getAverageADC();
    ADC_V= (averageADC-ADC0)*0.0000003;
    ui->label->setText(QString::number(averageADC,10));
    ui->label_3->setText(QString::number(ADC_V,'g',7));
}

void Electrometer_test::on_pushButton_clicked() //отмена
{
    if(test_source) //если тест был открыт из окна измерения
    {
        if(testOk)  //если тест пройден
        {
            close();
        }
        else
        {
            close();
            emit closeTestAll(); //посылаем сигнал закрыть другое окно
        }
    }
    else // если тест был вызван просто
    {
        close();
    }
}
void Electrometer_test::onCloseSlot() //прием сигнала о закрытии окна
{
    if(test_source) //если тест был открыт из окна измерения
    {
        if(testOk)  //если тест пройден
        {
            close();
        }
        else
        {
            close();
            emit closeTestAll(); //посылаем сигнал закрыть другое окно
        }
    }
    else // если тест был вызван просто
    {
        close();
    }
}

void Electrometer_test::testOkSlot()
{
    testOk=true;
}

