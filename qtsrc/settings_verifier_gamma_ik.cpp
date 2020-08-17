//**************************************** настройки поверителя - гамма излучение (ИК) *********************
#include "settings_verifier_gamma_ik.h"
#include "ui_settings_verifier_gamma_ik.h"

Settings_verifier_gamma_ik::Settings_verifier_gamma_ik(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings_verifier_gamma_ik)
{
    ui->setupUi(this);
    init();
}

void Settings_verifier_gamma_ik::init()
{
    setWindowTitle("Гамма излучение (ИК)");
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
        QString qStr="CREATE TABLE IF NOT EXISTS gammaChambers("
                                                              "name TEXT(10),"
                                                              "type TEXT(10),"
                                                              "volume TEXT(10),"
                                                              "sensibilityMAED TEXT(10),"
                                                              "sensibilityMNED TEXT(10),"
                                                              "sensibilityMIED TEXT(10),"
                                                              "sensibilityKERMA TEXT(10),"
                                                              "sensibilityMPD TEXT(10),"
                                                              "sensibilityMED TEXT(10),"
                                                              "sensibilityDimensionMAED TEXT(10),"
                                                              "sensibilityDimensionMNED TEXT(10),"
                                                              "sensibilityDimensionMIED TEXT(10),"
                                                              "sensibilityDimensionKERMA TEXT(10),"
                                                              "sensibilityDimensionMPD TEXT(10),"
                                                              "sensibilityDimensionMED TEXT(10),"
                                                              "voltage TEXT(10),"
                                                              "factoryNumber TEXT(10),"
                                                              "factoryDate TEXT(10),"
                                                              "verificationDate TEXT(10))";

        if(!query.exec(qStr)){qDebug()<<"unable execute query CREATE table betaChambers"<<query.lastError().text();}
        QString qStr1="SELECT * FROM gammaChambers";
        if(!query.exec(qStr1)){qDebug()<<"unable execute query SELECT *FROM table gammaChambers";}
        QStringList strList;
        while(query.next())   //выполняем запросы поочереди в цикле (начиная с первого)
        {
            strList.append(query.value("name").toString()+" дата поверки: "+query.value("VerificationDate").toString());
            //qDebug()<<"opa"<<query.value("name").toString()+" дата поверки: "+query.value("VerificationDate").toString();
        }
        ui->listWidget->addItems(strList); //выводим на экран
    }
}

Settings_verifier_gamma_ik::~Settings_verifier_gamma_ik()
{
    delete ui;
}

void Settings_verifier_gamma_ik::on_pushButton_4_clicked()//назад
{
    close();
}

void Settings_verifier_gamma_ik::on_pushButton_clicked() //добавить
{
    VerifyGammaChambers *verGammaChambers = new VerifyGammaChambers(this);
    verGammaChambers->setModal(true);
    verGammaChambers->setBut(0);
    verGammaChambers->setStyleSheet("QLabel{font-size:16px;}"
                              "QLineEdit{color:black; background-color:white; min-height:20px;font-size:18px;}");
    verGammaChambers->show();
    connect(verGammaChambers,&VerifyGammaChambers::addRefreshSignal,this,&Settings_verifier_gamma_ik::RefreshSignal);
}
void Settings_verifier_gamma_ik::RefreshSignal()
{
    ui->listWidget->clear();
    init();
}

void Settings_verifier_gamma_ik::on_pushButton_2_clicked() //правка
{
    QString str{""};
        if (ui->listWidget->currentItem()==nullptr) {return;}

        str=ui->listWidget->currentItem()->text();
        int x=str.indexOf(" "); // находим индекс первого пробела для поиска имени камеры
        VerifyGammaChambers *verGammaCham1 = new VerifyGammaChambers;
        verGammaCham1->setModal(true);
        verGammaCham1->setNameCh(str.left(x));
        verGammaCham1->setBut(1); //устанавливаем соответствие нажатия кнопки правка
        verGammaCham1->setStyleSheet("QLabel{font-size:16px;}"
                                  "QLineEdit{color:black; background-color:white; min-height:20px;font-size:18px;}");
        verGammaCham1->show();
        connect(verGammaCham1,&VerifyGammaChambers::addRefreshSignal,this,&Settings_verifier_gamma_ik::RefreshSignal);
}

void Settings_verifier_gamma_ik::on_pushButton_3_clicked() //удалить
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
        QString qStr="DELETE FROM gammaChambers WHERE name='%1'";
        QString qStr1=qStr.arg(chamberName);

        if(!query.exec(qStr1)) {qDebug()<<"unable execute query DELETE"<<query.lastError().text();}
        ui->listWidget->clear();
        init();
    }
}
