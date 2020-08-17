//************** Окно с результатами теста электрометра *****************************

#include "electrometer_test_result.h"
#include "ui_electrometer_test_result.h"

Electrometer_test_result::Electrometer_test_result(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Electrometer_test_result)
{
    ui->setupUi(this);
    init();
}

Electrometer_test_result::~Electrometer_test_result()
{
    delete ui;
}

void Electrometer_test_result::init()
{
    setWindowTitle("Результаты тестирования");

    ui->listWidget->setVisible(false);
    ui->pushButton_3->setEnabled(false);
    ui->pushButton_3->setVisible(false);
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
            ui->lineEdit->setText(query.value("R1").toString()); //заполняем первый lineEdit
        }
        qStr = "CREATE TABLE IF NOT EXISTS electrometerTest  (id INTEGER PRIMARY KEY CHECK (id=0),"   //таблица из одной строки
                                                             "Uoff TEXT(10),"
                                                             "DAC TEXT(10),"
                                                             "R1 TEXT(10),"
                                                             "R2 TEXT(10),"
                                                             "R3 TEXT(10),"
                                                             "Iin TEXT(10))";
        if(!query.exec(qStr)){qDebug()<<"unable execute query CREATE table electrometerTest"<<query.lastError().text();}
        //if(!query.exec("DROP TABLE electrometerTest")){qDebug()<<"unable execute query DELETE table electrometerTest"<<query.lastError().text();}
        qStr = "SELECT *FROM electrometerTest";
        if(!query.exec(qStr)){qDebug()<<"unable execute query SELECT from table electrometerTest"<<query.lastError().text();}
        if(query.first())
        {
            R3 = query.value("R3").toDouble();
            R2 = query.value("R2").toDouble();
            DAC = query.value("DAC").toDouble();
            RefVolt = query.value("Uoff").toDouble();
            int ADC=query.value("Iin").toInt();
            I = (ADC*0.0000003/(R3*1E9))*10E12;    //в пА;   (В/Ом=А *10^12 => пА)
            qDebug()<<ADC<<I<<R3;
            ui->lineEdit_5->setText(query.value("Uoff").toString());
            ui->lineEdit_6->setText(query.value("DAC").toString());
            ui->lineEdit->setText(query.value("R1").toString());
            ui->lineEdit_2->setText(query.value("R2").toString());
            ui->lineEdit_3->setText(query.value("R3").toString());
            ui->lineEdit_4->setText(QString::number(I,'g',4));     //выводим данные из БД
        }
        qStr="SELECT *FROM electrometer";
        if(!query.exec(qStr)){qDebug()<<"unable execute query SELECT from table electrometer"<<query.lastError().text();}
        if(query.first())
        {
            double minDAC = query.value("minDAC").toDouble();
            double maxDAC = query.value("maxDAC").toDouble();
            double minRefVolt = query.value("minRefVolt").toDouble();
            double maxRefVolt = query.value("maxRefVolt").toDouble();
            double minR2 = query.value("minR2").toDouble();
            double maxR2= query.value("maxR2").toDouble();
            double minR3 = query.value("minR3").toDouble();
            double maxR3 = query.value("maxR3").toDouble();
            double minI = query.value("minI").toDouble();
            double maxI = query.value("maxI").toDouble();
            bool errorFlag=false;
            if(RefVolt<minRefVolt||RefVolt>maxRefVolt)
            {
                errorFlag=true;
                ui->listWidget->setVisible(true);ui->listWidget->addItem("показания напряжения смещения усилителя вышли за пределы");
            }
            if(DAC<minDAC||DAC>maxDAC)
            {
                errorFlag=true;
                ui->listWidget->setVisible(true);ui->listWidget->addItem("показания ЦАП вышли за пределы");
            }
            if(R2<minR2||R2>maxR2)
            {
                errorFlag=true;
                ui->listWidget->setVisible(true);ui->listWidget->addItem("показания R2 вышли за пределы");
            }
            if(R3<minR3||R3>maxR3)
            {
                errorFlag=true;
                ui->listWidget->setVisible(true);ui->listWidget->addItem("показания R3 вышли за пределы");
            }
            if(I<minI||I>maxI)
            {
                errorFlag=true;
                ui->listWidget->setVisible(true);ui->listWidget->addItem("показания входного тока вышли за пределы");
            }
            if (errorFlag){ui->listWidget->addItem("Тест не пройден");}
        }
        db.close();
    }
}

void Electrometer_test_result::on_pushButton_2_clicked() //оk
{
    if(!ui->listWidget->isVisible()) //если окно с ошибками не появилось то посылаем сигнал о прохождении теста
    {
        emit testOkSig();
    }
    close();    
    emit onCloseSig();
}

void Electrometer_test_result::on_pushButton_3_clicked() //записать эталонные
{
    if(QSqlDatabase::contains("myDB"))
    {
        QSqlDatabase db=QSqlDatabase::database("myDB");
        db.setDatabaseName("config.db");
        if(!db.open()){qDebug()<<"db was not opened";}
        QSqlQuery query(db);
        QString qStr="INSERT INTO electrometerTest (id,"
                                     "Uoff ,"
                                     "DAC ,"
                                     "R1,"
                                     "R2,"
                                     "R3,"
                                     "Iin)"
             "VALUES(0,'%1','%2','%3','%4','%5','%6')";

        QString qStr1=qStr.arg(ui->lineEdit_5->text())
                          .arg(ui->lineEdit_6->text())
                          .arg(ui->lineEdit->text())
                          .arg(ui->lineEdit_2->text())
                          .arg(ui->lineEdit_3->text())
                          .arg(ui->lineEdit_4->text());
        if(!query.exec(qStr1)){qDebug()<<"unable execute query INSERT electrometerTest"<<query.lastError().text();}

        qStr="UPDATE electrometerTest SET "
                                     "Uoff='%1',"
                                     "DAC='%2' ,"
                                     "R1='%3' ,"
                                     "R2='%4',"
                                     "R3='%5',"
                                     "Iin='%6'";
        qStr1=qStr.arg(ui->lineEdit_5->text())
                  .arg(ui->lineEdit_6->text())
                  .arg(ui->lineEdit->text())
                  .arg(ui->lineEdit_2->text())
                  .arg(ui->lineEdit_3->text())
                  .arg(ui->lineEdit_4->text());

        if(!query.exec(qStr1)){qDebug()<<"unable execute query update electrometerTest"<<query.lastError().text();}
        db.close();
    }
}
