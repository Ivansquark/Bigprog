//**************************************** настройки поверителя - гамма излучение (БДГ) *********************

#include "settings_verifier_gamma_bdg.h"
#include "ui_settings_verifier_gamma_bdg.h"

Settings_verifier_gamma_bdg::Settings_verifier_gamma_bdg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings_verifier_gamma_bdg)
{
    ui->setupUi(this);
    init();
}

void Settings_verifier_gamma_bdg::init()
{
    int pass=Password::check();
    switch (pass)                   //проверяем 1 - поверитель или 2 - изготовитель
    {
       case 1:

                //ui->lineEdit_16->setEnabled(false);
                //ui->lineEdit_24->setEnabled(false);
                //ui->lineEdit_25->setEnabled(false);
                //ui->pushButton->setEnabled(false);
                //ui->pushButton->setVisible(false);
            break;
       case 2:

                //ui->lineEdit_16->setEnabled(true);
                //ui->lineEdit_24->setEnabled(true);
                //ui->lineEdit_25->setEnabled(true);
                //ui->pushButton->setEnabled(true);
                //ui->pushButton->setVisible(true);
            break;
            default:break;
    }    
    setWindowTitle("Блок детектирования гамма излучения (БДГ)");
    //if(ButStatus==0)
    //{
    //    ui->comboBox_7->addItem("(имп/с)/(мкЗв/ч)");
    //    ui->comboBox_8->addItem("(имп/с)/(мкЗв/ч)");
    //    ui->comboBox_9->addItem("(имп/с)/(мкЗв/ч)");
    //    ui->comboBox_10->addItem("(имп/с)/(мкГр/ч)");
    //    ui->comboBox_11->addItem("(имп/с)/(мкГр/ч)");
    //    ui->comboBox_12->addItem("(имп/с)/(мкР/ч)");
    //}

    QList<QComboBox*> comboList = this->findChildren<QComboBox*>();// все адреса комбобоксов запихиваем в Qlist
    for(QComboBox* i:comboList)
    {
        i->setFixedHeight(30);// вертикальный размер
        i->setStyleSheet("QComboBox{font-size:10px;}"); //размер шрифта
    }
    ui->lineEdit_16->setValidator(new QIntValidator(0,100,ui->lineEdit_16)); //Объем сцинтиллятора
    ui->lineEdit_17->setValidator(new QDoubleValidator(0.0, 1000.0, 5, ui->lineEdit_17));   //МАЭД
    ui->lineEdit_18->setValidator(new QDoubleValidator(0.0, 1000.0, 5, ui->lineEdit_18));   //МНЭД
    ui->lineEdit_19->setValidator(new QDoubleValidator(0.0, 1000.0, 5, ui->lineEdit_19));   //МИЭД
    ui->lineEdit_20->setValidator(new QDoubleValidator(0.0, 1000.0, 5, ui->lineEdit_20));   //КЕРМА
    ui->lineEdit_21->setValidator(new QDoubleValidator(0.0, 1000.0, 5, ui->lineEdit_21));   //МПД
    ui->lineEdit_22->setValidator(new QDoubleValidator(0.0, 1000.0, 5, ui->lineEdit_22));   //МЭД
    ui->lineEdit_25->setInputMask("00.00.0000");  //
    ui->lineEdit_26->setInputMask("00.00.0000"); // для даты


}

Settings_verifier_gamma_bdg::~Settings_verifier_gamma_bdg()
{
    delete ui;
}

void Settings_verifier_gamma_bdg::setBut(bool x)
{
    ButStatus=x;
    if(ButStatus==1)// правка (некоторые элементы делаем недоступными)
    {
        ui->lineEdit->setEnabled(false);ui->lineEdit_16->setEnabled(false);ui->lineEdit_24->setEnabled(false);
        ui->lineEdit_25->setEnabled(false);

        if(QSqlDatabase::contains("myDB"))  //создаем таблицу если не была создана, и выводим названия камер в listWidget
        {
            QSqlDatabase db=QSqlDatabase::database("myDB");
            db.setDatabaseName("config.db");
            if(!db.open()){qDebug()<<"db was not opened";}
            QSqlQuery query(db);
            QString qStr= "SELECT * FROM gammaBlock WHERE nuclide='%1'";
            QString qStr1 = qStr.arg(nameCh);
            if(!query.exec(qStr1)){qDebug()<<"unable execute query SELECT FROM table gammaBlock"<<query.lastError().text();}
            if(query.first())
            {
                ui->lineEdit->setText(query.value("nuclide").toString());
                ui->lineEdit_16->setText(query.value("volume").toString());
                ui->comboBox_7->addItem(query.value("sensibilityDimensionMAED").toString());
                ui->comboBox_8->addItem(query.value("sensibilityDimensionMNED").toString());
                ui->comboBox_9->addItem(query.value("sensibilityDimensionMIED").toString());
                ui->comboBox_10->addItem(query.value("sensibilityDimensionKERMA").toString());
                ui->comboBox_11->addItem(query.value("sensibilityDimensionMPD").toString());
                ui->comboBox_12->addItem(query.value("sensibilityDimensionMED").toString());
                ui->label_36->setText(query.value("sensibilityMAED").toString());
                ui->label_38->setText(query.value("sensibilityMNED").toString());
                ui->label_40->setText(query.value("sensibilityMIED").toString());
                ui->label_42->setText(query.value("sensibilityKERMA").toString());
                ui->label_44->setText(query.value("sensibilityMPD").toString());
                ui->label_46->setText(query.value("sensibilityMED").toString());
                ui->lineEdit_24->setText(query.value("factoryNumber").toString());
                ui->lineEdit_25->setText(query.value("factoryDate").toString());
                ui->lineEdit_26->setText(query.value("verificationDate").toString());

                ui->lineEdit_17->setText(QString::number(ui->label_36->text().toDouble()/3.6e9,'f',3));
                ui->lineEdit_18->setText(QString::number(ui->label_38->text().toDouble()/3.6e9,'f',3));
                ui->lineEdit_19->setText(QString::number(ui->label_40->text().toDouble()/3.6e9,'f',3));
                ui->lineEdit_20->setText(QString::number(ui->label_42->text().toDouble()/3.6e9,'f',3));
                ui->lineEdit_21->setText(QString::number(ui->label_44->text().toDouble()/3.6e9,'f',3));
                ui->lineEdit_22->setText(QString::number(ui->label_46->text().toDouble()/3.6e9,'f',3));
            }
        }
    }
    else if(ButStatus==0) //добавить
    {
        ui->lineEdit->setEnabled(true);
        ui->lineEdit_16->setEnabled(true);
        ui->lineEdit_24->setEnabled(true);
        ui->lineEdit_25->setEnabled(true);

        ui->comboBox_7->addItem("(имп/с)/(мкЗв/ч)");
        ui->comboBox_8->addItem("(имп/с)/(мкЗв/ч)");
        ui->comboBox_9->addItem("(имп/с)/(мкЗв/ч)");
        ui->comboBox_10->addItem("(имп/с)/(мкГр/ч)");
        ui->comboBox_11->addItem("(имп/с)/(мкГр/ч)");
        ui->comboBox_12->addItem("(имп/с)/(мкР/ч)");
    }
}

void Settings_verifier_gamma_bdg::setNameCh(QString str)
{
    nameCh=str;
    qDebug()<<nameCh;
}

void Settings_verifier_gamma_bdg::on_pushButton_4_clicked()//назад
{    
    close();
}

void Settings_verifier_gamma_bdg::on_lineEdit_17_textEdited(const QString &arg1) //МАЭД
{
    QString str=arg1;
    str.replace(",",".");  // меняем запятые на точки (точки не выводятся для данных параметров ввода, да и так лучше, чтобы вводилось что-то одно)
    double x=str.toDouble();
    ui->label_36->setText(QString::number(x*3.6e9,'f',5)); //коэффициент переводит (имп/с)/(мкЗв/ч)  в (имп/с)/(Зв/с)
}

void Settings_verifier_gamma_bdg::on_lineEdit_18_textEdited(const QString &arg1) //МНЭД
{
    QString str=arg1;
    str.replace(",",".");  // меняем запятые на точки (точки не выводятся для данных параметров ввода, да и так лучше, чтобы вводилось что-то одно)
    double x=str.toDouble();
    ui->label_38->setText(QString::number(x*3.6e9,'f',5)); //коэффициент переводит (имп/с)/(мкЗв/ч)  в (имп/с)/(Зв/с)
}

void Settings_verifier_gamma_bdg::on_lineEdit_19_textEdited(const QString &arg1) //МИЭД
{
    QString str=arg1;
    str.replace(",",".");  // меняем запятые на точки (точки не выводятся для данных параметров ввода, да и так лучше, чтобы вводилось что-то одно)
    double x=str.toDouble();
    ui->label_40->setText(QString::number(x*3.6e9,'f',5)); //коэффициент переводит (имп/с)/(мкЗв/ч)  в (имп/с)/(Зв/с)
}

void Settings_verifier_gamma_bdg::on_lineEdit_20_textEdited(const QString &arg1) //КЕРМА
{
    QString str=arg1;
    str.replace(",",".");  // меняем запятые на точки (точки не выводятся для данных параметров ввода, да и так лучше, чтобы вводилось что-то одно)
    double x=str.toDouble();
    ui->label_42->setText(QString::number(x*3.6e9,'f',5)); //коэффициент переводит (имп/с)/(мкГр/ч)  в (имп/с)/(Гр/с)
}

void Settings_verifier_gamma_bdg::on_lineEdit_21_textEdited(const QString &arg1) //МПД
{
    QString str=arg1;
    str.replace(",",".");  // меняем запятые на точки (точки не выводятся для данных параметров ввода, да и так лучше, чтобы вводилось что-то одно)
    double x=str.toDouble();
    ui->label_44->setText(QString::number(x*3.6e9,'f',5)); //коэффициент переводит (имп/с)/(мкГр/ч)  в (имп/с)/(Гр/с)
}

void Settings_verifier_gamma_bdg::on_lineEdit_22_textEdited(const QString &arg1) //МЭД
{
    QString str=arg1;
    str.replace(",",".");  // меняем запятые на точки (точки не выводятся для данных параметров ввода, да и так лучше, чтобы вводилось что-то одно)
    double x=str.toDouble();
    ui->label_46->setText(QString::number(x*3.6e9,'f',5)); //коэффициент переводит (имп/с)/(мкР/ч)  в (имп/с)/(Р/с)
}

void Settings_verifier_gamma_bdg::on_pushButton_3_clicked() //Ок
{
    //добавить
    if(ButStatus==0)
    {
        bool emptyFlag=false;                                       //объявляем и инициализируем флаг проверки на пустые строки
        QList<QLineEdit*> lst = this->findChildren<QLineEdit*>();   //все lineEdit-ы кладем в QList
        for(QLineEdit* i:lst)                                       //в цикле проверяем на пустоту
        {
            if(i->text().isEmpty()){emptyFlag=true;}                //если есть пустые строки, выставляем флаг
        }

        QString date_string_on_db = ui->lineEdit_25->text();                 //Проверка даты на валидность
        QString date_string_on_db1 = ui->lineEdit_26->text();                // заносим в строки
        QDate date = QDate::fromString(date_string_on_db1,"dd.MM.yyyy");    // приводим к объектам date
        QDate date1 = QDate::fromString(date_string_on_db1,"dd.MM.yyyy");   // и date1
        if(!date.isValid()||!date1.isValid())                               // и проверяем, если таких дат нет выставляем флаг
        {
            emptyFlag=true;
        }
        if(!emptyFlag)   //если флаг равен false
        {
            if(QSqlDatabase::contains("myDB"))
            {
                QSqlDatabase db=QSqlDatabase::database("myDB");
                db.setDatabaseName("config.db");
                if(!db.open()){qDebug()<<"db was not opened";}
                QSqlQuery query(db);
                QString qStr="INSERT INTO gammaBlock (nuclide,"
                                             "volume ,"
                                             "sensibilityMAED ,"
                                             "sensibilityMNED ,"
                                             "sensibilityMIED,"
                                             "sensibilityKERMA,"
                                             "sensibilityMPD,"
                                             "sensibilityMED,"
                                             "sensibilityDimensionMAED,"
                                             "sensibilityDimensionMNED,"
                                             "sensibilityDimensionMIED,"
                                             "sensibilityDimensionKERMA,"
                                             "sensibilityDimensionMPD,"
                                             "sensibilityDimensionMED,"
                                             "factoryNumber,"
                                             "factoryDate,"
                                             "verificationDate)"
                     "VALUES('%1','%2','%3','%4','%5','%6','%7','%8','%9','%10','%11','%12','%13','%14','%15','%16','%17')";
                QString qStr1=qStr.arg(ui->lineEdit->text())
                                  .arg(ui->lineEdit_16->text())
                                  .arg(ui->label_36->text())
                                  .arg(ui->label_38->text())
                                  .arg(ui->label_40->text())
                                  .arg(ui->label_42->text())
                                  .arg(ui->label_44->text())
                                  .arg(ui->label_46->text())
                                  .arg(ui->comboBox_7->currentText())
                                  .arg(ui->comboBox_8->currentText())
                                  .arg(ui->comboBox_9->currentText())
                                  .arg(ui->comboBox_10->currentText())
                                  .arg(ui->comboBox_11->currentText())
                                  .arg(ui->comboBox_12->currentText())
                                  .arg(ui->lineEdit_24->text())
                                  .arg(ui->lineEdit_25->text())
                                  .arg(ui->lineEdit_26->text());
                if(!query.exec(qStr1)){qDebug()<<"unable execute query INSERT"<<query.lastError().text();}
                db.close();
                close();
                emit addRefreshSignal();
            }
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

        QString date_string_on_db = ui->lineEdit_25->text();                 //Проверка даты на валидность
        QString date_string_on_db1 = ui->lineEdit_26->text();                // заносим в строки
        QDate date = QDate::fromString(date_string_on_db1,"dd.MM.yyyy");    // приводим к объектам date
        QDate date1 = QDate::fromString(date_string_on_db1,"dd.MM.yyyy");   // и date1
        if(!date.isValid()||!date1.isValid())                               // и проверяем, если таких дат нет выставляем флаг
        {
            emptyFlag=true;
        }
        if(!emptyFlag)   //если флаг равен false
        {
            if(QSqlDatabase::contains("myDB"))
            {
                QSqlDatabase db=QSqlDatabase::database("myDB");
                db.setDatabaseName("config.db");
                if(!db.open()){qDebug()<<"db was not opened";}
                QSqlQuery query(db);
                QString qStr="UPDATE gammaBlock SET volume='%1',"
                                             "sensibilityMAED='%2',"
                                             "sensibilityMNED='%3' ,"
                                             "sensibilityMIED='%4',"
                                             "sensibilityKERMA='%5',"
                                             "sensibilityMPD='%6',"
                                             "sensibilityMED='%7',"
                                             "sensibilityDimensionMAED='%8',"
                                             "sensibilityDimensionMNED='%9',"
                                             "sensibilityDimensionMIED='%10',"
                                             "sensibilityDimensionKERMA='%11',"
                                             "sensibilityDimensionMPD='%12',"
                                             "sensibilityDimensionMED='%13',"
                                             "verificationDate='%14'"
                             "WHERE nuclide='%15'";
                QString qStr1=qStr.arg(ui->lineEdit_16->text())
                                  .arg(ui->label_36->text())
                                  .arg(ui->label_38->text())
                                  .arg(ui->label_40->text())
                                  .arg(ui->label_42->text())
                                  .arg(ui->label_44->text())
                                  .arg(ui->label_46->text())
                                  .arg(ui->comboBox_7->currentText())
                                  .arg(ui->comboBox_8->currentText())
                                  .arg(ui->comboBox_9->currentText())
                                  .arg(ui->comboBox_10->currentText())
                                  .arg(ui->comboBox_11->currentText())
                                  .arg(ui->comboBox_12->currentText())
                                  .arg(ui->lineEdit_26->text())
                                  .arg(nameCh);
                if(!query.exec(qStr1)){qDebug()<<"unable execute query UPDATE"<<query.lastError().text();}
                db.close();
                emit addRefreshSignal();
                close();
            }
        }
    }
}


