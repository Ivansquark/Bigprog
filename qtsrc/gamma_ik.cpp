//******************* "Гамма излучение -ионизационные камеры" ***********************

#include "gamma_ik.h"
#include "ui_gamma_ik.h"

Gamma_IK::Gamma_IK(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Gamma_IK)
{
    ui->setupUi(this);
    init();
}

Gamma_IK::~Gamma_IK()
{
    delete ui;
}

void Gamma_IK::init()
{
    setWindowTitle("Гамма излучение -ионизационные камеры");
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

void Gamma_IK::on_pushButton_clicked() // назад
{
    close();
}

void Gamma_IK::on_pushButton_2_clicked() //тест аппаратуры
{    
    Electrometer_test* elTest = new Electrometer_test(this);
    elTest->setModal(true);
    elTest->show();
}

void Gamma_IK::on_pushButton_3_clicked() //измерение
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
    Gamma_IK_measuring* gammaIKmes = new Gamma_IK_measuring(this);
    gammaIKmes->setModal(true);
    gammaIKmes->show();
}
