//**************************************** настройки изготовителя - настройки электрометра *********************

#include "settings_electrometer.h"
#include "ui_settings_electrometer.h"

Settings_electrometer::Settings_electrometer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings_electrometer)
{
    ui->setupUi(this);
    init();
}

Settings_electrometer::~Settings_electrometer()
{
    delete ui;
}

void Settings_electrometer::init()
{
    setWindowTitle("Настройки электрометра");
    ui->pushButton_3->setEnabled(false);
    ui->pushButton_3->setVisible(false); //делаем невидимой кнопку добавления строки
    if(QSqlDatabase::contains("myDB"))  //создаем таблицу если не была создана, и выводим названия камер в listWidget
    {
        QSqlDatabase db=QSqlDatabase::database("myDB");
        db.setDatabaseName("config.db");
        if(!db.open()){qDebug()<<"db was not opened";}
        QSqlQuery query(db);
        QString qStr="CREATE TABLE IF NOT EXISTS electrometer(   id INTEGER PRIMARY KEY CHECK (id=0),"   //таблица из одной строки
                                                              "periodRS232 TEXT(10),"
                                                              "R1 TEXT(10),"
                                                              "referenceVoltage TEXT(10),"
                                                              "heatingTime TEXT(10),"
                                                              "changeTimeout TEXT(10),"
                                                              "testTimeout TEXT(10),"
                                                              "countsAver TEXT(10),"
                                                              "minDAC TEXT(10),"
                                                              "maxDAC TEXT(10),"
                                                              "minRefVolt TEXT(10),"
                                                              "maxRefVolt TEXT(10),"
                                                              "minR2 TEXT(10),"
                                                              "maxR2 TEXT(10),"
                                                              "minR3 TEXT(10),"
                                                              "maxR3 TEXT(10),"
                                                              "minI TEXT(10),"
                                                              "maxI TEXT(10))";
        if(!query.exec(qStr)){qDebug()<<"unable execute query CREATE table electrometer"<<query.lastError().text();}
        //if(!query.exec("DROP TABLE electrometer")){qDebug()<<"unable execute query DELETE table electrometer"<<query.lastError().text();}
        qStr = "SELECT *FROM electrometer";
        if(!query.exec(qStr)){qDebug()<<"unable execute query SELECT table electrometer"<<query.lastError().text();}
        if(query.first())
        {
            ui->lineEdit->setText(query.value("periodRS232").toString());
            ui->lineEdit_2->setText(query.value("R1").toString());
            ui->lineEdit_3->setText(query.value("referenceVoltage").toString());
            ui->lineEdit_4->setText(query.value("heatingTime").toString());
            ui->lineEdit_5->setText(query.value("changeTimeout").toString());
            ui->lineEdit_6->setText(query.value("testTimeout").toString());
            ui->lineEdit_7->setText(query.value("countsAver").toString());
            ui->lineEdit_8->setText(query.value("minDAC").toString());
            ui->lineEdit_9->setText(query.value("maxDAC").toString());
            ui->lineEdit_10->setText(query.value("minRefVolt").toString());
            ui->lineEdit_11->setText(query.value("maxRefVolt").toString());
            ui->lineEdit_12->setText(query.value("minR2").toString());
            ui->lineEdit_13->setText(query.value("maxR2").toString());
            ui->lineEdit_14->setText(query.value("minR3").toString());
            ui->lineEdit_15->setText(query.value("maxR3").toString());
            ui->lineEdit_17->setText(query.value("minI").toString());
            ui->lineEdit_16->setText(query.value("maxI").toString());
        }
    }
}

void Settings_electrometer::on_pushButton_2_clicked() //назад
{
    close();
}

void Settings_electrometer::on_pushButton_clicked() //Ок
{
    if(QSqlDatabase::contains("myDB"))  //создаем таблицу если не была создана, и выводим названия камер в listWidget
    {
        QSqlDatabase db=QSqlDatabase::database("myDB");
        db.setDatabaseName("config.db");
        if(!db.open()){qDebug()<<"db was not opened";}
        QSqlQuery query(db);
        QString qStr="UPDATE electrometer SET "
                                     "periodRS232='%1',"
                                     "R1='%2' ,"
                                     "referenceVoltage='%3',"
                                     "heatingTime='%4',"
                                     "changeTimeout='%5',"
                                     "testTimeout='%6',"
                                     "countsAver='%7',"
                                     "minDAC='%8',"
                                     "maxDAC='%9',"
                                     "minRefVolt='%10',"
                                     "maxRefVolt='%11',"
                                     "minR2='%12',"
                                     "maxR2='%13',"
                                     "minR3='%14',"
                                     "maxR3='%15',"
                                     "minI='%16',"
                                     "maxI='%17'";
        QString qStr1=qStr.arg(ui->lineEdit->text())
                          .arg(ui->lineEdit_2->text())
                          .arg(ui->lineEdit_3->text())
                          .arg(ui->lineEdit_4->text())
                          .arg(ui->lineEdit_5->text())
                          .arg(ui->lineEdit_6->text())
                          .arg(ui->lineEdit_7->text())
                          .arg(ui->lineEdit_8->text())
                          .arg(ui->lineEdit_9->text())
                          .arg(ui->lineEdit_10->text())
                          .arg(ui->lineEdit_11->text())
                          .arg(ui->lineEdit_12->text())
                          .arg(ui->lineEdit_13->text())
                          .arg(ui->lineEdit_14->text())
                          .arg(ui->lineEdit_15->text())
                          .arg(ui->lineEdit_17->text())
                          .arg(ui->lineEdit_16->text());
        if(!query.exec(qStr1)){qDebug()<<"unable execute query update"<<query.lastError().text();}
    }
    close();
}

void Settings_electrometer::on_pushButton_3_clicked() //INSERT - скрытая кнопка (нужна для заполнения первой строки таблицы)
{
    if(QSqlDatabase::contains("myDB"))  //создаем таблицу если не была создана, и выводим названия камер в listWidget
    {
        QSqlDatabase db=QSqlDatabase::database("myDB");
        db.setDatabaseName("config.db");
        if(!db.open()){qDebug()<<"db was not opened";}
        QSqlQuery query(db);
        QString qStr="INSERT INTO electrometer (id,"
                                     "periodRS232 ,"
                                     "R1,"
                                     "referenceVoltage,"
                                     "heatingTime,"
                                     "changeTimeout,"
                                     "testTimeout,"
                                     "countsAver,"
                                     "minDAC,"
                                     "maxDAC,"
                                     "minRefVolt,"
                                     "maxRefVolt,"
                                     "minR2,"
                                     "maxR2,"
                                     "minR3,"
                                     "maxR3,"
                                     "minI,"
                                     "maxI)"
             "VALUES(0,'%1','%2','%3','%4','%5','%6','%7','%8','%9','%10','%11','%12','%13','%14','%15','%16','%17')";
        QString qStr1=qStr.arg(ui->lineEdit->text())
                          .arg(ui->lineEdit_2->text())
                          .arg(ui->lineEdit_3->text())
                          .arg(ui->lineEdit_4->text())
                          .arg(ui->lineEdit_5->text())
                          .arg(ui->lineEdit_6->text())
                          .arg(ui->lineEdit_7->text())
                          .arg(ui->lineEdit_8->text())
                          .arg(ui->lineEdit_9->text())
                          .arg(ui->lineEdit_10->text())
                          .arg(ui->lineEdit_11->text())
                          .arg(ui->lineEdit_12->text())
                          .arg(ui->lineEdit_13->text())
                          .arg(ui->lineEdit_14->text())
                          .arg(ui->lineEdit_15->text())
                          .arg(ui->lineEdit_17->text())
                          .arg(ui->lineEdit_16->text());
        if(!query.exec(qStr1)){qDebug()<<"unable execute query INSERT"<<query.lastError().text();}
    }
}
