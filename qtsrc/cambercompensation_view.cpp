//*********************************** класс в котором отображается процесс компенсации *****************

#include "cambercompensation_view.h"
#include "ui_cambercompensation_view.h"

CamberCompensation_view::CamberCompensation_view(QWidget *parent,int voltage) :
    QDialog(parent),
    ui(new Ui::CamberCompensation_view), voltage(voltage)
{
    ui->setupUi(this);
    init();
}

CamberCompensation_view::~CamberCompensation_view()
{
    delete averageADC;
    delete ui;
}

void CamberCompensation_view::init()
{
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle("Выполнение компенсации");
    QByteArray byteVolt;
    voltHex volthex;
    byteVolt=volthex.intVolt(voltage); // - через отдельный класс переводим напряжение в цифрах в hex код для посылки в СОМ порт (7х8х)
    //qDebug()<<QString::number(byteVolt[0],16)<<QString::number(byteVolt[1],16);
    //qDebug()<<voltage<<" "<<byteVolt.toHex();
    averageADC=new AverageADC;           // запустим СОМ порт
    averageADC->sendToComSlot1(byteVolt); //подадим высокое напряжение

    timer = new QTimer(this);
    connect(timer,&QTimer::timeout,this,&CamberCompensation_view::timeOut);
    delayTimer = new QTimer(this);
    connect(delayTimer,&QTimer::timeout,this,&CamberCompensation_view::delayTimeOut);
    delayTimer->setSingleShot(true);
    delayTimer->start(50000);        //дадим время на прогрев
    QByteArray hC2;
    //hC2[2]={0x2C,0x32},
    hC2[0]=0x2C;//28
    hC2[1]=0x32;//32 код реле - К1+К8 - Чувствительный диапазон
    averageADC->sendToComSlot(hC2);
    for(int i=0;i<5000000;i++){;}
    ui->label->setText("Прогрев");
    averageADC->sendToComSlot(hC2);


    if(QSqlDatabase::contains("myDB"))
    {
        QSqlDatabase db=QSqlDatabase::database("myDB");
        db.setDatabaseName("config.db");
        if(!db.open()){qDebug()<<"db was not opened";}
        QSqlQuery query(db);
        QString qStr = "SELECT *FROM electrometerTest";
        if(!query.exec(qStr)){qDebug()<<"unable execute query SELECT from table electrometerTest"<<query.lastError().text();}
        if(query.first())
        {
            Ioff=query.value("Iin").toDouble();
        }
        db.close();
    }

}

void CamberCompensation_view::delay(int x)
{
    while (x>0) x--;
}

void CamberCompensation_view::on_pushButton_clicked()// отмена
{
    close();
}

void CamberCompensation_view::delayTimeOut()
{
    timer->start(1000);
}

void CamberCompensation_view::timeOut()
{
    time+=1;
    ADC=averageADC->getAverageADC()-ADC0 -Ioff; //показания АЦП за вычетом значения АЦП для токов утечки электрометра без камер определенного при тесте
    ui->label_2->setText(QString::number(ADC));
        if (time==1)
        {
            ui->label->setText("Измерения на чувствительном диапазоне");
            QByteArray hC2;
            //hC2[2]={0x2C,0x32},
            hC2[0]=0x2C;//28
            hC2[1]=0x32;//32 код реле - К1+К8 - Чувствительный диапазон
            averageADC->sendToComSlot(hC2);
            delay(10000);
            averageADC->sendToComSlot(hC2);
            ui->progressBar->setValue(0);
        }
        if (time>=2 && time<=16)
        {
             ui->progressBar->setValue(10 + 5*(int(time)-2)); //доп прогрев
        }

        if (time>=16 && time<=20)                           //рассчет
        {
            ui->progressBar->setValue(10 + 5*(int(time)-2));
            y+=ADC;
            qDebug()<<y;
            if (time==20)
            {
               ADC_V=y/5;
               Comp1=static_cast<int>(round(ADC_V));
               qDebug()<<"Comp1"<<Comp1;
               y=0;
            }
        }
        if (time==21)
        {
            ui->label->setText("Измерения на среднем диапазоне");
            QByteArray h90;
            h90[0]=0x29;//29
            h90[1]=0x30;//30 код реле - К3+К8 - Средний диапазон (на среднем диапазоне фильтр шумов не подключается)
            averageADC->sendToComSlot(h90);
            delay(10000);
            averageADC->sendToComSlot(h90);
            ui->progressBar->setValue(0);
        }
        if (time>=22 && time<=36)
        {
             ui->progressBar->setValue(10 + 5*(int(time)-22));
        }
        if (time>=36 && time<=40)
        {
            ui->progressBar->setValue(10 + 5*(int(time)-22));
            y+=ADC;
            qDebug()<<y;
            if (time==40)
            {
               ADC_V=y/5;
               Comp2=static_cast<int>(round(ADC_V));
               qDebug()<<"Comp2"<<Comp2;
               y=0;
            }
        }
        if (time==41)
        {
            ui->label->setText("Измерения на грубом диапазоне");
            QByteArray hA0;
            hA0[0]=0x2a;//2a
            hA0[1]=0x30;//30 код реле - К5+К7+К8 - Грубый диапазон + фильтр шумов (С1 - 560пФ) (на грубом диапазоне фильтр шумов не отключается)
            averageADC->sendToComSlot(hA0);
            delay(10000);
            averageADC->sendToComSlot(hA0);
            ui->progressBar->setValue(0);
        }
        if (time>=42 && time<=56)
        {
             ui->progressBar->setValue(10 + 5*(int(time)-42));
        }
        if (time>=56 && time<=60)
        {
            ui->progressBar->setValue(10 + 5*(int(time)-42));
            y+=ADC;
            qDebug()<<y;
            if (time==60)
            {
               ADC_V=y/5;
               Comp3=static_cast<int>(round(ADC_V));
               qDebug()<<"Comp3"<<Comp3;
               y=0;
            }
        }
        if (time==61) ui->progressBar->setValue(100);
        if (time==62)
        {
            close();
            emit compPar(Comp1,Comp2,Comp3);
            //delete this;
        }
        //qDebug()<<T;
}



