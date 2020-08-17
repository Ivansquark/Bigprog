//********************* Нейтронное излучение  ***************************

#include "neutron.h"
#include "ui_neutron.h"

Neutron::Neutron(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Neutron)
{
    ui->setupUi(this);
    init();
}

Neutron::~Neutron()
{
    delete ui;
}

void Neutron::init()
{
    setWindowTitle("Нейтронное излучение");
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

void Neutron::on_pushButton_clicked() // назад
{
    close();
}

void Neutron::on_pushButton_2_clicked() //тест аппаратуры
{
    Neutron_BDKN_test * neutronTest = new Neutron_BDKN_test(this);
    neutronTest->setModal(true);
    neutronTest->show();
}

void Neutron::on_pushButton_3_clicked() //измерение
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
    Neutron_measuring* neutronMes = new Neutron_measuring(this);
    neutronMes->setModal(true);
    neutronMes->show();
}
