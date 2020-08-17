#include "gamma_bdg.h"
#include "ui_gamma_bdg.h"
//#include "<QSerialPortInfo>

Gamma_BDG::Gamma_BDG(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Gamma_BDG)
{
    ui->setupUi(this);
    init();
}

Gamma_BDG::~Gamma_BDG()
{
    delete ui;
}

void Gamma_BDG::init()
{
    setWindowTitle("Гамма излучение - блок БДГ");
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

void Gamma_BDG::on_pushButton_clicked()//назад
{
    close();
}

void Gamma_BDG::on_pushButton_2_clicked() //БДГ - тест аппаратуры
{
    Gamma_BDG_test* gammaBDGtest = new Gamma_BDG_test;
    gammaBDGtest->setModal(true);
    gammaBDGtest->show();
    //QSerialPortInfo::
}

void Gamma_BDG::on_pushButton_3_clicked() //измерение
{
    if(QSqlDatabase::contains("myDB"))
    {
        QSqlDatabase db = QSqlDatabase::database("myDB");
        db.setDatabaseName("config.db");
        if(!db.open())
        {qDebug()<<"DB was not opened";}
        QSqlQuery query(db);
        QString queryStr;
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
    Gamma_BDG_measuring* gammaBDGmes = new Gamma_BDG_measuring(this);
    gammaBDGmes->setModal(true);
    gammaBDGmes->show();
}
