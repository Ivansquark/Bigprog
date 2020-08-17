#include "settings_verifier_gamma_nuclides.h"
#include "ui_settings_verifier_gamma_nuclides.h"

Settings_verifier_gamma_nuclides::Settings_verifier_gamma_nuclides(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings_verifier_gamma_nuclides)
{
    ui->setupUi(this);
    init();
}

Settings_verifier_gamma_nuclides::~Settings_verifier_gamma_nuclides()
{
    delete ui;
}

void Settings_verifier_gamma_nuclides::init()
{
    setWindowTitle("Настройки поверителя - БДГ - нуклиды");

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
        QString qStr="CREATE TABLE IF NOT EXISTS gammaBlock(   nuclide TEXT(10),"
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
                                                              "factoryNumber TEXT(10),"
                                                              "factoryDate TEXT(10),"
                                                              "verificationDate TEXT(10))";
        if(!query.exec(qStr)){qDebug()<<"unable execute query CREATE table gammaBlock"<<query.lastError().text();}
        //if(!query.exec("DROP TABLE gammaBlock")){qDebug()<<"unable execute query DROP table gammaBlock"<<query.lastError().text();}

        QString qStr1="SELECT * FROM gammaBlock";
        if(!query.exec(qStr1)){qDebug()<<"unable execute query SELECT *FROM table gammaBlock";}
        QStringList strList;
        while(query.next())   //выполняем запросы поочереди в цикле (начиная с первого)
        {
            strList.append(query.value("nuclide").toString()+" дата поверки: "+query.value("verificationDate").toString());
            //qDebug()<<"opa"<<query.value("name").toString()+" дата поверки: "+query.value("verificationDate").toString();
        }
        ui->listWidget->addItems(strList); //выводим на экран
        //ui->listWidget->update();
        db.close();
    }
}

void Settings_verifier_gamma_nuclides::on_pushButton_4_clicked() //назад
{
    close();
}

void Settings_verifier_gamma_nuclides::on_pushButton_clicked() //добавить
{
    Settings_verifier_gamma_bdg* setVerGammaBDG = new Settings_verifier_gamma_bdg(this);
    setVerGammaBDG->setModal(true);
    setVerGammaBDG->setBut(0);
    setVerGammaBDG->setStyleSheet("QLabel{font-size:16px;}"
                              "QLineEdit{color:black; background-color:white; min-height:20px;font-size:18px;}");
    setVerGammaBDG->show();
}

void Settings_verifier_gamma_nuclides::on_pushButton_2_clicked() //правка
{
    QString str{""};
    if (ui->listWidget->currentItem()==nullptr) {return;}//если не выделен Item то покидаем функцию (такое бывает если удалить камеру)

    str=ui->listWidget->currentItem()->text();
    int x=str.indexOf(" "); // находим индес первого пробела (строка до него соответствует имени камеры)
                            // по этому имени и ищем параметры камеры в следующем окне


    Settings_verifier_gamma_bdg* setVerGammaBDG = new Settings_verifier_gamma_bdg(this);
    setVerGammaBDG->setModal(true);
    setVerGammaBDG->setNameCh(str.left(x));
    setVerGammaBDG->setBut(1);
    setVerGammaBDG->setStyleSheet("QLabel{font-size:16px;}"
                              "QLineEdit{color:black; background-color:white; min-height:20px;font-size:18px;}");
    setVerGammaBDG->show();
    connect(setVerGammaBDG,&Settings_verifier_gamma_bdg::addRefreshSignal,this,&Settings_verifier_gamma_nuclides::addRefreshSlot);
}

void Settings_verifier_gamma_nuclides::addRefreshSlot()
{
    ui->listWidget->clear();
    init();
}

void Settings_verifier_gamma_nuclides::on_pushButton_3_clicked() //удалить
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
        QString qStr="DELETE FROM gammaBlock WHERE nuclide='%1'";
        QString qStr1=qStr.arg(chamberName);

        if(!query.exec(qStr1)) {qDebug()<<"unable execute query DELETE"<<query.lastError().text();}
        ui->listWidget->clear();
        init();
    }
}
