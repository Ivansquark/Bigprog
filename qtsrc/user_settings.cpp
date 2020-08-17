//***************************** настройки пользователей ***********************************************

#include "user_settings.h"
#include "ui_user_settings.h"

user_settings::user_settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::user_settings)
{
    ui->setupUi(this);
    init();
}

user_settings::~user_settings()
{
    delete ui;
}

void user_settings::init()
{
    setWindowTitle("Настройки пользователей");
    bool CurrSource=0;
    if(QSqlDatabase::contains("myDB"))
    {
        QSqlDatabase db=QSqlDatabase::database("myDB");
        db.setDatabaseName("config.db");
        if(!db.open()){qDebug()<<"db was not opened";}
        QSqlQuery query(db);
        QString qStr = "CREATE TABLE IF NOT EXISTS SourceChoice (id INTEGER PRIMARY KEY CHECK (id=0),"             //создаем новую таблицу из одной строки т.к (id=0)
                                                    "emulator BOOLEAN)";
        if(!query.exec(qStr)){qDebug()<<"unable execute query CREATE table SourceChoice"<<query.lastError().text();}
        //qStr = "INSERT INTO SourceChoice (id,emulator) VALUES (0,1)";
        //if(!query.exec(qStr)){qDebug()<<"unable execute query INSERT in table SourceChoice"<<query.lastError().text();}

        qStr = "SELECT *FROM electrometer";
        if(!query.exec(qStr)){qDebug()<<"unable execute query SELECT table electrometer"<<query.lastError().text();}

        qStr = "SELECT *FROM SourceChoice";
        if(!query.exec(qStr)){qDebug()<<"unable execute query SELECT table electrometer"<<query.lastError().text();}
        if(query.first())
        {
            CurrSource = (query.value("emulator").toBool()); //получаем значение из БД 1-эмулятор 0-детектор
        }
        db.close();
    }
    if (CurrSource==true)
    {
        ui->radioButton->setChecked(true);   //выствляем кнопку на эмуляторе
    }
    else
    {
        ui->radioButton_2->setChecked(true); //выствляем кнопку на детекторе
    }
}

void user_settings::on_pushButton_clicked() //назад
{
    if(QSqlDatabase::contains("myDB"))
    {
        QSqlDatabase db=QSqlDatabase::database("myDB");
        db.setDatabaseName("config.db");
        if(!db.open()){qDebug()<<"db was not opened";}
        QSqlQuery query(db);
        QString qStr ="";
        if (ui->radioButton->isChecked())
        {
            qStr="UPDATE SourceChoice SET emulator=1";  //если выбран эмулятор записать в переменную emulator 1
        }
        else {qStr="UPDATE SourceChoice SET emulator=0";} //выбран детектор
        if(!query.exec(qStr)){qDebug()<<"unable execute query UPDATE table SourceChoice"<<query.lastError().text();}
        db.close();
    }

    close();
}

void user_settings::on_pushButton_2_clicked() // Изменить пароль поверителя
{
    settings_verifier_change_password * setPasswordV = new settings_verifier_change_password(this);
    setPasswordV->setModal(true);
    setPasswordV->show();
}
