//*************************************** Настройки поверителя - нейтронные блоки ************************************

#include "verifyneutron.h"
#include "ui_verifyneutron.h"

VerifyNeutron::VerifyNeutron(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VerifyNeutron)
{
    ui->setupUi(this);
    init();
}

VerifyNeutron::~VerifyNeutron()
{
    delete ui;
}

void VerifyNeutron::setBut(int x)
{
    ButStatus=x;
    if(ButStatus==1)// правка (некоторые элементы делаем недоступными)
    {
        ui->lineEdit->setEnabled(false);ui->lineEdit_2->setEnabled(false);ui->lineEdit_3->setEnabled(false);
                       ui->lineEdit_6->setEnabled(false);ui->lineEdit_7->setEnabled(false);
    }
    else if(ButStatus==0) //добавить
    {
        ui->lineEdit->setEnabled(true);ui->lineEdit_2->setEnabled(true);ui->lineEdit_3->setEnabled(true);
                       ui->lineEdit_6->setEnabled(true);ui->lineEdit_7->setEnabled(true);
    }
}


void VerifyNeutron::init()
{
    setWindowTitle("Блоки детектирования нейтронного излучения");
    ui->comboBox->addItem("(имп/с)/(н/(см.кв.*с)");
    ui->comboBox_2->addItem("(имп/с)/(н/(см.кв.*с)");
    ui->comboBox_3->addItem("(имп/с)/(мкГр/час)");
    ui->comboBox_4->addItem("(имп/с)/(мкГр/час)");
    ui->comboBox_5->addItem("(имп/с)/(мкЗв/час)");
    ui->comboBox_6->addItem("(имп/с)/(мкЗв/час)");
    QList<QComboBox*> comboList = this->findChildren<QComboBox*>();// все адреса комбобоксов запихиваем в Qlist
    for(QComboBox* i:comboList)
    {
        i->setFixedHeight(20);// вертикальный размер
        i->setStyleSheet("QComboBox{font-size:12px;}"); //размер шрифта
    }
    QRegExp expName;
    expName.setPattern(QString("[а-яА-Я]+[-А-Я0-9]{10}"));
    QRegExpValidator *rx=new QRegExpValidator(expName,this);
    ui->lineEdit->setValidator(rx);//Название Блока
    ui->lineEdit_3->setValidator(new QIntValidator(0,100,ui->lineEdit_3)); //Толщина замедлителя
    ui->lineEdit_4->setValidator(new QDoubleValidator(0.0, 1000.0, 5, ui->lineEdit_4));     //ПП без Cd
    ui->lineEdit_9->setValidator(new QDoubleValidator(0.0, 1000.0, 5, ui->lineEdit_9));     //ПП с Cd
    ui->lineEdit_10->setValidator(new QDoubleValidator(0.0, 1000.0, 5, ui->lineEdit_10));   //МПД без Cd
    ui->lineEdit_11->setValidator(new QDoubleValidator(0.0, 1000.0, 5, ui->lineEdit_11));   //МПД с Cd
    ui->lineEdit_12->setValidator(new QDoubleValidator(0.0, 1000.0, 5, ui->lineEdit_12));   //МАЭД без Cd
    ui->lineEdit_13->setValidator(new QDoubleValidator(0.0, 1000.0, 5, ui->lineEdit_13));   //МАЭД с Cd

    ui->lineEdit_7->setInputMask("00.00.0000");  //
    ui->lineEdit_8->setInputMask("00.00.0000"); // для даты

}

void VerifyNeutron::setNameCh(QString str)
{
    nameCh=str;
        if(ButStatus==1)// правка (по имени камеры находим ее в БД и заполняем поля параметрами камеры)
        {
            QSqlDatabase db=QSqlDatabase::database("myDB");
            db.setDatabaseName("config.db");
            if(!db.open()){qDebug()<<"db was not opened";}
            QSqlQuery query(db);
            QString qStr = "SELECT * FROM neutronBlocks WHERE name='%1'";
            QString qStr1 = qStr.arg(nameCh);
            if(!query.exec(qStr1)){qDebug()<<"unable execute query SELECT FROM neutronBlocks WHERE name='%1";}
            if(query.first())
            {
                ui->lineEdit->setText(query.value("name").toString());
                ui->lineEdit_2->setText(query.value("energy").toString());
                ui->lineEdit_3->setText(query.value("width").toString());
                ui->lineEdit_4->setText(QString::number(query.value("sensibilityPPwithoutCd").toDouble(),'f',5));

                //ui->lineEdit_5->setText(query.value("voltage").toString());
                ui->lineEdit_6->setText(query.value("factoryNumber").toString());
                ui->lineEdit_7->setText(query.value("factoryDate").toString());
                ui->lineEdit_8->setText(query.value("verificationDate").toString());
                ui->lineEdit_9->setText(QString::number(query.value("sensibilityPPwithCd").toDouble(),'f',5));
                ui->lineEdit_10->setText(QString::number(query.value("sensibilityMPDwithoutCd").toDouble()/(3.6e9),'f',5));
                ui->lineEdit_11->setText(QString::number(query.value("sensibilityMPDwithCd").toDouble()/(3.6e9),'f',5));
                ui->lineEdit_12->setText(QString::number(query.value("sensibilityMAEDwithoutCd").toDouble()/(3.6e9),'f',5));
                ui->lineEdit_13->setText(QString::number(query.value("sensibilityMAEDwithCd").toDouble()/(3.6e9),'f',5));
                ui->label_23->setText(query.value("sensibilityMAEDwithCd").toString());
                ui->label_25->setText(query.value("sensibilityMAEDwithoutCd").toString());
                ui->label_17->setText(query.value("sensibilityMPDwithCd").toString());
                ui->label_5->setText(query.value("sensibilityMPDwithoutCd").toString());
                ui->label_19->setText(query.value("sensibilityPPwithoutCd").toString());
                ui->label_21->setText(query.value("sensibilityPPwithCd").toString());
            }
        }
}

void VerifyNeutron::on_pushButton_2_clicked()//назад
{
    close();
}



void VerifyNeutron::on_pushButton_clicked()//ок
{
    if(ButStatus==0)//добавить
    {
        bool emptyFlag=false;                                       //объявляем и инициализируем флаг проверки на пустые строки
        QList<QLineEdit*> lst = this->findChildren<QLineEdit*>();   //все lineEdit-ы кладем в QList
        for(QLineEdit* i:lst)                                       //в цикле проверяем на пустоту
        {
            if(i->text().isEmpty()){emptyFlag=true;}                //если есть пустые строки, выставляем флаг
        }

        QString date_string_on_db = ui->lineEdit_7->text();                 //Проверка даты на валидность
        QString date_string_on_db1 = ui->lineEdit_8->text();                // заносим в строки
        QDate date = QDate::fromString(date_string_on_db1,"dd.MM.yyyy");    // приводим к объектам date
        QDate date1 = QDate::fromString(date_string_on_db1,"dd.MM.yyyy");   // и date1
        if(!date.isValid()||!date1.isValid())                               // и проверяем, если таких дат нет выставляем флаг
        {
            emptyFlag=true;
        }
        if(!emptyFlag)   //если флаг равен false
        {
            QSqlDatabase db=QSqlDatabase::database("myDB");
            db.setDatabaseName("config.db");
            if(!db.open()){qDebug()<<"db was not opened";}
            QSqlQuery query(db);
            QString qStr="INSERT INTO neutronBlocks("
                         "name,"
                         "energy,"
                         "width,"
                         "sensibilityPPwithoutCd,"
                         "sensibilityPPwithCd,"
                         "sensibilityMPDwithoutCd,"
                         "sensibilityMPDwithCd,"
                         "sensibilityMAEDwithoutCd,"
                         "sensibilityMAEDwithCd,"
                         "sensibilityDimensionPPwithoutCd,"
                         "sensibilityDimensionPPwithCd,"
                         "sensibilityDimensionMPDwithoutCd,"
                         "sensibilityDimensionMPDwithCd,"
                         "sensibilityDimensionMAEDwithoutCd,"
                         "sensibilityDimensionMAEDwithCd,"
                         "factoryNumber,"
                         "factoryDate,"
                         "verificationDate)"
                         "VALUES ('%1','%2','%3','%4','%5','%6','%7','%8','%9','%10',"
                                 "'%11','%12','%13','%14','%15','%16','%17','%18')";
            QString qStr1=qStr.arg(ui->lineEdit->text()).                           //запрос на вставку новой строки в БД
                               arg(ui->lineEdit_2->text()).                         //новые параметры берем из lineEdit-ов
                               arg(ui->lineEdit_3->text()).
                               arg(ui->label_19->text()).
                               arg(ui->label_21->text()).
                               arg(ui->label_5->text()).
                               arg(ui->label_17->text()).
                               arg(ui->label_25->text()).
                               arg(ui->label_23->text()).
                               arg(ui->comboBox->currentText()).
                               arg(ui->comboBox_2->currentText()).
                               arg(ui->comboBox_3->currentText()).
                               arg(ui->comboBox_4->currentText()).
                               arg(ui->comboBox_5->currentText()).
                               arg(ui->comboBox_6->currentText()).
                               arg(ui->lineEdit_6->text()).
                               arg(ui->lineEdit_7->text()).
                               arg(ui->lineEdit_8->text());
            if(!query.exec(qStr1)){qDebug()<<"unable execute query INSERT INTO table neutronBlocks"<<query.lastError().text();}
            db.close();
            emit addRefreshSignal(); //посылаем сигнал об обновлении списка камер;
            close();
        }
    }
    if(ButStatus==1)//правка
    {
        bool emptyFlag=false;                                       //объявляем и инициализируем флаг проверки на пустые строки
        QList<QLineEdit*> lst = this->findChildren<QLineEdit*>();   //все lineEdit-ы кладем в QList
        for(QLineEdit* i:lst)                                       //в цикле проверяем на пустоту
        {
            if(i->text().isEmpty()){emptyFlag=true;}                //если есть пустые строки, выставляем флаг
        }

        QString date_string_on_db = ui->lineEdit_7->text();                 //Проверка даты на валидность
        QString date_string_on_db1 = ui->lineEdit_8->text();                // заносим в строки
        QDate date = QDate::fromString(date_string_on_db1,"dd.MM.yyyy");    // приводим к объектам date
        QDate date1 = QDate::fromString(date_string_on_db1,"dd.MM.yyyy");   // и date1
        if(!date.isValid()||!date1.isValid())                               // и проверяем, если таких дат нет выставляем флаг
        {
            emptyFlag=true;
        }
        if(!emptyFlag)   //если флаг равен false
        {

            QSqlDatabase db=QSqlDatabase::database("myDB");
            db.setDatabaseName("config.db");
            if(!db.open()){qDebug()<<"db was not opened";}
            QSqlQuery query(db);
            QString qStr="UPDATE neutronBlocks SET sensibilityPPwithoutCd='%1',sensibilityDimensionPPwithoutCd='%2',sensibilityPPwithCd='%3',"
                    "sensibilityDimensionPPwithCd='%4',sensibilityMPDwithoutCd='%5', sensibilityDimensionMPDwithoutCd='%6', sensibilityMPDwithCd='%7',"
                    "sensibilityDimensionMPDwithCd='%8', sensibilityMAEDwithoutCd='%9', sensibilityDimensionMAEDwithoutCd='%10', sensibilityMAEDwithCd='%11',"
                    "sensibilityDimensionMAEDwithCd='%12',verificationDate='%13'WHERE name='%14'";
            QString qStr1=qStr.arg(ui->label_19->text()).
                               arg(ui->comboBox->currentText()).
                               arg(ui->label_21->text()).
                               arg(ui->comboBox_2->currentText()).
                               arg(ui->label_5->text()).
                               arg(ui->comboBox_3->currentText()).
                               arg(ui->label_17->text()).
                               arg(ui->comboBox_4->currentText()).
                               arg(ui->label_25->text()).
                               arg(ui->comboBox_5->currentText()).
                               arg(ui->label_23->text()).
                               arg(ui->comboBox_6->currentText()).
                               arg(ui->lineEdit_8->text()).
                               arg(ui->lineEdit->text());
            if(!query.exec(qStr1)){qDebug()<<"unable execute query UPDATE neutronBlocks"<<query.lastError().text();}
            db.close();
            emit addRefreshSignal(); //посылаем сигнал об обновлении списка камер;
            close();
        }
        else
        {
            QMessageBox msg;
            msg.setIcon(QMessageBox::Icon::Warning);
            msg.setStandardButtons(QMessageBox::StandardButton::Ok);
            msg.setWindowTitle("Внимание");
            msg.setText("Имеются некорректно заданные параметры");
            msg.exec();
        }
    }
    emit addRefreshSignal();
}

void VerifyNeutron::on_lineEdit_4_textEdited(const QString &arg1) //ПП без Cd
{
    QString str=arg1;
    str.replace(",",".");  // меняем запятые на точки (точки не выводятся для данных параметров ввода, да и так лучше, чтобы вводилось что-то одно)
    double x=str.toDouble();
    ui->label_19->setText(QString::number(x*1,'f',5)); //коэффициент переводит (имп/с)/(н/(см.кв.*с)  в (имп/с)/(н/(см.кв.*с)
}

void VerifyNeutron::on_lineEdit_9_textEdited(const QString &arg1) //ПП с Cd
{
    QString str=arg1;
    str.replace(",",".");  // меняем запятые на точки (точки не выводятся для данных параметров ввода, да и так лучше, чтобы вводилось что-то одно)
    double x=str.toDouble();
    ui->label_21->setText(QString::number(x*1,'f',5)); //коэффициент переводит (имп/с)/(н/(см.кв.*с)  в (имп/с)/(н/(см.кв.*с)
}


void VerifyNeutron::on_lineEdit_10_textEdited(const QString &arg1) //МПД без Cd
{
    QString str=arg1;
    str.replace(",",".");  // меняем запятые на точки (точки не выводятся для данных параметров ввода, да и так лучше, чтобы вводилось что-то одно)
    double x=str.toDouble();
    ui->label_5->setText(QString::number(x*3.6e9,'f',5)); //коэффициент переводит (имп/с)/(мкГр/час) в (имп/с)/(Гр/с)
}

void VerifyNeutron::on_lineEdit_11_textEdited(const QString &arg1) //МПД с Cd
{
    QString str=arg1;
    str.replace(",",".");  // меняем запятые на точки (точки не выводятся для данных параметров ввода, да и так лучше, чтобы вводилось что-то одно)
    double x=str.toDouble();
    ui->label_17->setText(QString::number(x*3.6e9,'f',5)); //коэффициент переводит (имп/с)/(мкГр/час) в (имп/с)/(Гр/с)
}

void VerifyNeutron::on_lineEdit_12_textEdited(const QString &arg1) //МАЭД без Cd
{
    QString str=arg1;
    str.replace(",",".");  // меняем запятые на точки (точки не выводятся для данных параметров ввода, да и так лучше, чтобы вводилось что-то одно)
    double x=str.toDouble();
    ui->label_25->setText(QString::number(x*3.6e9,'f',5)); //коэффициент переводит (имп/с)/(мкЗв/час) в (имп/с)/(Зв/с)
}

void VerifyNeutron::on_lineEdit_13_textEdited(const QString &arg1) //МАЭД с Cd
{
    QString str=arg1;
    str.replace(",",".");  // меняем запятые на точки (точки не выводятся для данных параметров ввода, да и так лучше, чтобы вводилось что-то одно)
    double x=str.toDouble();
    ui->label_23->setText(QString::number(x*3.6e9,'f',5)); //коэффициент переводит (имп/с)/(мкЗв/час) в (имп/с)/(Зв/с)
}
