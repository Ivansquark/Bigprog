//**************************************** настройки поверителя - бета излучение (ИК) *********************
#include "settings_verifier_beta.h"
#include "ui_settings_verifier_beta.h"

Settings_verifier_beta::Settings_verifier_beta(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings_verifier_beta)
{
    ui->setupUi(this);
    init();
}

Settings_verifier_beta::~Settings_verifier_beta()
{
    delete ui;
}

void Settings_verifier_beta::init()
{
    setWindowTitle("Бета излучение (ИК)");
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
        QString qStr="CREATE TABLE IF NOT EXISTS betaChambers("
                                                              "name TEXT(10),"
                                                              "diameter TEXT(10),"
                                                              "elSpace TEXT(10),"
                                                              "sensibility TEXT(10),"
                                                              "sensibilityDimension TEXT(10),"
                                                              "voltage TEXT(10),"
                                                              "factoryNumber TEXT(10),"
                                                              "factoryDate TEXT(10),"
                                                              "verificationDate TEXT(10))";

        if(!query.exec(qStr)){qDebug()<<"unable execute query CREATE table betaChambers";}
        QString qStr1="SELECT * FROM betaChambers";
        if(!query.exec(qStr1)){qDebug()<<"unable execute query SELECT *FROM table betaChambers";}
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
void Settings_verifier_beta::on_pushButton_4_clicked() //назад
{
    close();
}

void Settings_verifier_beta::on_pushButton_clicked() //добавить
{
    VerifyBetaChambers *verBetaCham = new VerifyBetaChambers(this);
    verBetaCham->setModal(true);
    verBetaCham->setBut(0); //устанавливаем соответствие нажатия кнопки добавить
    verBetaCham->show();
    connect(verBetaCham,&VerifyBetaChambers::addRefreshSignal,this,&Settings_verifier_beta::addRefreshSlot);
}

void Settings_verifier_beta::addRefreshSlot()
{
    ui->listWidget->clear();
    init();                     //заново вызываем функцию инициализации объекта (нашего окна с listWidget-ом)
}

void Settings_verifier_beta::on_pushButton_2_clicked() //правка
{
    QString str{""};
    if (ui->listWidget->currentItem()==nullptr) {return;}//если не выделен Item то покидаем функцию (такое бывает если удалить камеру)

    str=ui->listWidget->currentItem()->text();
    int x=str.indexOf(" "); // находим индес первого пробела (строка до него соответствует имени камеры)
                            // по этому имени и ищем параметры камеры в следующем окне

    VerifyBetaChambers *verBetaCham1 = new VerifyBetaChambers;
    verBetaCham1->setModal(true);
    verBetaCham1->setNameCh(str.left(x)); //обрезаем строку до первого пробела и передаем методу, чтобы указать какую камеру мы выбрали
    verBetaCham1->setBut(1); //устанавливаем соответствие нажатия кнопки правка
    verBetaCham1->show();
    connect(verBetaCham1,&VerifyBetaChambers::addRefreshSignal,this,&Settings_verifier_beta::addRefreshSlot);
}

void Settings_verifier_beta::on_pushButton_3_clicked()//удалить
{
    QString currentName="";
    QString chamberName="";
    if(ui->listWidget->currentItem()==nullptr){return;} //
    currentName = ui->listWidget->currentItem()->text();
    int x=currentName.indexOf(" "); //ищем индекс первого пробела
    chamberName = currentName.left(x); //обрезаем до пробела (получаем имя камеры)
    // можно написать и так в одну строку:
    //  chamberName=ui->listWidget->currentItem()->text().left(ui->listWidget->currentItem()->text().indexOf(" "));
    // но так очень сложно для восприятия и лучше избегать подобных конструкций для облегчения понимания кода
    //
    if(QSqlDatabase::contains("myDB"))
    {
        QSqlDatabase db = QSqlDatabase::database("myDB");
        db.setDatabaseName("config.db");
        if(!db.open()){qDebug()<<"db was not opened";}
        QSqlQuery query(db);
        QString qStr="DELETE FROM betaChambers WHERE name='%1'";
        QString qStr1=qStr.arg(chamberName);
        if(!query.exec(qStr1)) {qDebug()<<"unable execute query DELETE"<<query.lastError().text();}//проверка ошибок сильно помогает
        ui->listWidget->clear();                                                                   //в SQL запросах, т.к. строгий синтаксис
        init(); //заново вызываем функцию инициализации объекта (нашего окна с listWidget-ом)
    }
}
