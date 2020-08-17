//*************************** Окно бета измерения *********************************


#include "beta.h"
#include "ui_beta.h"

Beta::Beta(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Beta)
{
    ui->setupUi(this);
    init();
}

Beta::~Beta()
{
    //delete betaMes;
    //delete elTest;//не нужно удалять т.к. удалится сам
    delete ui;
}

void Beta::on_pushButton_clicked() //назад
{
    close();
}

void Beta::init()
{
    setWindowTitle("Бета излучение");
    setAttribute(Qt::WA_DeleteOnClose);
    if(QSqlDatabase::contains("myDB"))
    {
        QSqlDatabase db = QSqlDatabase::database("myDB");
        db.setDatabaseName("config.db");
        if(!db.open()){qDebug()<<"DB was not opened";}
        QSqlQuery query(db);
        QString queryStr = "SELECT * FROM fullName";
        if(!query.exec(queryStr))
        {qDebug()<<"unable execute query SELECT";}
        if(query.first())
        {
            ui->lineEdit->setText(query.value("surname").toString());
            ui->lineEdit_2->setText(query.value("name").toString());
            ui->lineEdit_3->setText(query.value("patronymic").toString());
        }
    }
}

void Beta::on_pushButton_2_clicked() //тест аппаратуры
{    
    elTest = new Electrometer_test(this);
    elTest->setModal(true);
    elTest->show();
}

void Beta::on_pushButton_3_clicked() //измерение
{
    /*!TODO: <БД ФИО*/
    if(QSqlDatabase::contains("myDB"))
    {
        QSqlDatabase db = QSqlDatabase::database("myDB");
        db.setDatabaseName("config.db");
        if(!db.open())
        {qDebug()<<"DB was not opened";}
        QSqlQuery query(db);
        QString queryStr;
        //if(!query.exec("DROP TABLE fullName"))
        //{qDebug()<<"unable execute query drop"<<query.lastError().text();}
        //queryStr = "CREATE TABLE IF NOT EXISTS fullName "
        //                   "(id INTEGER PRIMARY KEY CHECK (id=0),"
        //                   "surname TEXT(10),"
        //                   "name TEXT(10),"
        //                   "patronymic TEXT(10))";
        //if(!query.exec(queryStr))
        //{qDebug()<<"unable execute query CREATE"<<query.lastError().text();}
        //QString queryStr1 = "INSERT INTO fullName (id, surname, name, patronymic)"
        //                    "VALUES (0, '1', '2', '3')";
        //
        //if(!query.exec(queryStr1))
        //{qDebug()<<"unable execute query INSERT";}
        QString surname(ui->lineEdit->text());
        QString name(ui->lineEdit_2->text());
        QString patronimyc(ui->lineEdit_3->text());
        QString queryStr1 = "UPDATE fullName SET "
                    "surname = '%1',"
                    "name = '%2',"
                    "patronymic = '%3'";
        queryStr = queryStr1.arg(surname)
                            .arg(name)
                            .arg(patronimyc);
        if(!query.exec(queryStr))
        {qDebug()<<"unable execute query UPDATE";}
    }

    betaMes = new Beta_measuring(this);
    betaMes->setModal(true);
    betaMes->show();
}
