//*************************** Класс проверки пароля *********************************

#include "password.h"

Password::Password()
{}

int Password::check() //ф-я проверки введенного пароля (статическая)
{
    QString passCurrent{"1"}; //переменная в которую запишется текущий введенный пароль
    QString passManufacturer{""};
    QString passVerifier{""};

    if(QSqlDatabase::contains("myDB"))
    {
        QSqlDatabase db=QSqlDatabase::database("myDB");
        db.setDatabaseName("config.db");
        if(!db.open()){qDebug()<<"db was not opened";}
        QSqlQuery query(db);
        QString passQuery = "SELECT * FROM password";
        if(!query.exec(passQuery))                    //выполнить запрос на выбор всего(*) из таблицы password
        {qDebug()<<"unable execute query SELECT"<<query.lastError().text();}
        if(query.first()) // обращение к первой строке таблицы
        {
            passCurrent=query.value("passCurrent").toString();
            passManufacturer=query.value("passManufacturer").toString();
            passVerifier=query.value("passVerifier").toString();
        }
        db.close();
    }
    if(passCurrent==passManufacturer)       // проверяем введенный пароль
    {
        return 2;                           //2 - изготовитель
    }
    else if (passCurrent==passVerifier)
    {
        return 1;                           //1- поверитель
    }
    else
    {
        return 0;
    }
}
