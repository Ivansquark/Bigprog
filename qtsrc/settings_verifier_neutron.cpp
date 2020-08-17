//**************************************** настройки поверителя - нейтронное излучение *********************

#include "settings_verifier_neutron.h"
#include "ui_settings_verifier_neutron.h"

Settings_verifier_neutron::Settings_verifier_neutron(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings_verifier_neutron)
{
    ui->setupUi(this);
    init();
}

void Settings_verifier_neutron::init()
{
    setWindowTitle("Блоки нейтронного излучения");
    int pass=Password::check(); //вызываем статическую функцию класса Password (статическую, чтобы не создавать объекты)
    switch (pass)                   //проверяем 1 - поверитель или 2 - изготовитель
    {
       case 2: ui->pushButton->setVisible(true);ui->pushButton->setEnabled(true);
               ui->pushButton_3->setVisible(true);ui->pushButton_3->setEnabled(true);
            break;
       case 1: ui->pushButton->setVisible(false);ui->pushButton->setEnabled(false);
               ui->pushButton_3->setVisible(false);ui->pushButton_3->setEnabled(false);
            break;
            default:break;
    }

    if(QSqlDatabase::contains("myDB"))  //создаем таблицу если не была создана, и выводим названия камер в listWidget
    {
        QSqlDatabase db=QSqlDatabase::database("myDB");
        db.setDatabaseName("config.db");
        if(!db.open()){qDebug()<<"db was not opened";}
        QSqlQuery query(db);
        QString qStr="CREATE TABLE IF NOT EXISTS neutronBlocks("
                                                               "name TEXT(10),"
                                                               "energy TEXT(10),"
                                                               "width TEXT(10),"
                                                               "sensibilityPPwithoutCd TEXT(10),"
                                                               "sensibilityPPwithCd TEXT(10),"
                                                               "sensibilityMPDwithoutCd TEXT(10),"
                                                               "sensibilityMPDwithCd TEXT(10),"
                                                               "sensibilityMAEDwithoutCd TEXT(10),"
                                                               "sensibilityMAEDwithCd TEXT(10),"
                                                               "sensibilityDimensionPPwithoutCd TEXT(10),"
                                                               "sensibilityDimensionPPwithCd TEXT(10),"
                                                               "sensibilityDimensionMPDwithoutCd TEXT(10),"
                                                               "sensibilityDimensionMPDwithCd TEXT(10),"
                                                               "sensibilityDimensionMAEDwithoutCd TEXT(10),"
                                                               "sensibilityDimensionMAEDwithCd TEXT(10),"
                                                               "factoryNumber TEXT(10),"
                                                               "factoryDate TEXT(10),"
                                                               "verificationDate TEXT(10))";

        if(!query.exec(qStr)){qDebug()<<"unable execute query CREATE table neutronBlocks"<<query.lastError().text();}
        QString qStr1="SELECT * FROM neutronBlocks";
        if(!query.exec(qStr1)){qDebug()<<"unable execute query SELECT *FROM table neutronBlocks";}
        QStringList strList;
        while(query.next())   //выполняем запросы поочереди в цикле (начиная с первого)
        {
            strList.append(query.value("name").toString()+" дата поверки: "+query.value("VerificationDate").toString());
            //qDebug()<<"opa"<<query.value("name").toString()+" дата поверки: "+query.value("VerificationDate").toString();
        }
        ui->listWidget->addItems(strList); //выводим на экран
        //ui->listWidget->update();
    }    
}

Settings_verifier_neutron::~Settings_verifier_neutron()
{
    delete ui;
}

void Settings_verifier_neutron::addRefresh()
{
    ui->listWidget->clear();
    init();
}

void Settings_verifier_neutron::on_pushButton_4_clicked()//назад
{
    close();
}

void Settings_verifier_neutron::on_pushButton_clicked() //добавить
{
    VerifyNeutron* verNeutron = new VerifyNeutron(this);
    verNeutron->setModal(true);
    verNeutron->setBut(0);
    verNeutron->setStyleSheet("QLabel{font-size:16px;}"
                              "QLineEdit{color:black; background-color:white; min-height:20px;font-size:18px;}");
    verNeutron->show();
    connect(verNeutron,&VerifyNeutron::addRefreshSignal,this,&Settings_verifier_neutron::addRefresh);
}

void Settings_verifier_neutron::on_pushButton_2_clicked()//ПРАВКА
{
    QString str{""};
            if (ui->listWidget->currentItem()==nullptr) {return;}

            str=ui->listWidget->currentItem()->text();
            int x=str.indexOf(" "); // находим индекс первого пробела для поиска имени камеры
            VerifyNeutron *verNeutron1 = new VerifyNeutron;
            verNeutron1->setModal(true);
            verNeutron1->setNameCh(str.left(x));
            verNeutron1->setBut(1); //устанавливаем соответствие нажатия кнопки правка
            verNeutron1->setStyleSheet("QLabel{font-size:16px;}"
                                      "QLineEdit{color:black; background-color:white; min-height:20px;font-size:18px;}");
            verNeutron1->show();
            connect(verNeutron1,&VerifyNeutron::addRefreshSignal,this,&Settings_verifier_neutron::addRefresh);
}

void Settings_verifier_neutron::on_pushButton_3_clicked() //удалить
{
    QString currentName="";
        QString chamberName="";
        if(ui->listWidget->currentItem()==nullptr){return;}
        currentName = ui->listWidget->currentItem()->text();
        int x=currentName.indexOf(" ");
        chamberName = currentName.left(x); //Имя - слово до первого пробела

        if(QSqlDatabase::contains("myDB"))
        {
            QSqlDatabase db = QSqlDatabase::database("myDB");
            db.setDatabaseName("config.db");
            if(!db.open()){qDebug()<<"db was not opened";}

            QSqlQuery query(db);
            QString qStr="DELETE FROM neutronBlocks WHERE name='%1'";
            QString qStr1=qStr.arg(chamberName);

            if(!query.exec(qStr1)) {qDebug()<<"unable execute query DELETE"<<query.lastError().text();}
            ui->listWidget->clear();
            init();
        }
}
