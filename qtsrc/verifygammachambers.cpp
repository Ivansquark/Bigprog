//*************************************** Настройки поверителя - Гамма камеры ************************************

#include "verifygammachambers.h"
#include "ui_verifygammachambers.h"

VerifyGammaChambers::VerifyGammaChambers(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VerifyGammaChambers)
{
    ui->setupUi(this);
    init();
}

VerifyGammaChambers::~VerifyGammaChambers()
{
    delete ui;
}

void VerifyGammaChambers::init()
{
    setWindowTitle("Камеры гамма излучения");
    ui->comboBox->addItem("мЗв/нКл");
    ui->comboBox_2->addItem("мЗв/нКл");
    ui->comboBox_3->addItem("мЗв/нКл");
    ui->comboBox_4->addItem("мГр/нКл");
    ui->comboBox_5->addItem("мГр/нКл");
    ui->comboBox_6->addItem("мР/нКл");
    QRegExp expName;
    expName.setPattern(QString("[а-яА-Я]+[-0-9]{10}"));
    QRegExpValidator *rx=new QRegExpValidator(expName,this);
    ui->lineEdit->setValidator(rx);
    ui->lineEdit_3->setValidator(new QDoubleValidator(0,5000.0,4,ui->lineEdit_3));
    ui->lineEdit_4->setValidator(new QDoubleValidator(0.0, 1000.0, 5, ui->lineEdit_4));     //МАЭД
    ui->lineEdit_9->setValidator(new QDoubleValidator(0.0, 1000.0, 5, ui->lineEdit_9));     //МНЭД
    ui->lineEdit_10->setValidator(new QDoubleValidator(0.0, 1000.0, 5, ui->lineEdit_10));   //МИЭД
    ui->lineEdit_11->setValidator(new QDoubleValidator(0.0, 1000.0, 5, ui->lineEdit_11));   //Мощность КЕРМА
    ui->lineEdit_12->setValidator(new QDoubleValidator(0.0, 1000.0, 5, ui->lineEdit_12));   //МПД
    ui->lineEdit_13->setValidator(new QDoubleValidator(0.0, 1000.0, 5, ui->lineEdit_13));   //МЭД
    ui->lineEdit_5->setValidator(new QIntValidator(-400,400,ui->lineEdit_5)); //только int  напряжение

    ui->lineEdit_7->setInputMask("00.00.0000");  //
    ui->lineEdit_8->setInputMask("00.00.0000"); // для даты
}

void VerifyGammaChambers::on_pushButton_2_clicked()//назад
{
    close();
}

void VerifyGammaChambers::on_pushButton_clicked() //Ок
{
    if(ButStatus==0) //добавить
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
            QString qStr="INSERT INTO gammaChambers("
                         "name,"
                         "type,"
                         "volume,"
                         "sensibilityMAED,"
                         "sensibilityMNED,"
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
                         "voltage,"
                         "factoryNumber,"
                         "factoryDate,"
                         "verificationDate)"
                         "VALUES ('%1','%2','%3','%4','%5','%6','%7','%8','%9','%10',"
                                 "'%11','%12','%13','%14','%15','%16','%17','%18','%19')";
            QString qStr1=qStr.arg(ui->lineEdit->text()).                           //запрос на вставку новой строки в БД
                               arg(ui->lineEdit_2->text()).                         //новые параметры берем из lineEdit-ов
                               arg(ui->lineEdit_3->text()).
                               arg(ui->label_19->text()).
                               arg(ui->label_12->text()).
                               arg(ui->label_14->text()).
                               arg(ui->label_16->text()).
                               arg(ui->label_18->text()).
                               arg(ui->label_22->text()).
                               arg(ui->comboBox->currentText()).
                               arg(ui->comboBox_2->currentText()).
                               arg(ui->comboBox_3->currentText()).
                               arg(ui->comboBox_4->currentText()).
                               arg(ui->comboBox_5->currentText()).
                               arg(ui->comboBox_6->currentText()).
                               arg(ui->lineEdit_5->text()).
                               arg(ui->lineEdit_6->text()).
                               arg(ui->lineEdit_7->text()).
                               arg(ui->lineEdit_8->text());
            if(!query.exec(qStr1)){qDebug()<<"unable execute query INSERT INTO table gammaChambers"<<query.lastError().text();}
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
            msg.setText("Имеются не корректно заданные параметры");
            msg.exec();
        }
    }
    else if(ButStatus==1) // правка
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
            QString qStr="UPDATE gammaChambers SET sensibilityMAED='%1',sensibilityDimensionMAED='%2',sensibilityMNED='%3',"
                    "sensibilityDimensionMNED='%4',sensibilityMIED='%5', sensibilityDimensionMIED='%6', sensibilityKERMA='%7',"
                    "sensibilityDimensionKERMA='%8', sensibilityMPD='%9', sensibilityDimensionMPD='%10', sensibilityMED='%11',"
                    "sensibilityDimensionMED='%12', voltage='%13',verificationDate='%14' WHERE name='%15'";
            QString qStr1=qStr.arg(ui->label_19->text()).
                               arg(ui->comboBox->currentText()).
                               arg(ui->label_12->text()).
                               arg(ui->comboBox_2->currentText()).
                               arg(ui->label_14->text()).
                               arg(ui->comboBox_3->currentText()).
                               arg(ui->label_16->text()).
                               arg(ui->comboBox_4->currentText()).
                               arg(ui->label_18->text()).
                               arg(ui->comboBox_5->currentText()).
                               arg(ui->label_22->text()).
                               arg(ui->comboBox_6->currentText()).
                               arg(ui->lineEdit_5->text()).
                               arg(ui->lineEdit_8->text()).
                               arg(ui->lineEdit->text());

            if(!query.exec(qStr1)){qDebug()<<"unable execute query UPDATE gammaChambers"<<query.lastError().text();}
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
}

void VerifyGammaChambers::setBut(int x)  //устанавливаем флаг о редактировании=1 или добавлении=2
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

void VerifyGammaChambers::setNameCh(QString str)
{
    nameCh=str;
    if(ButStatus==1)// правка (по имени камеры находим ее в БД и заполняем поля параметрами камеры)
    {
        QSqlDatabase db=QSqlDatabase::database("myDB");
        db.setDatabaseName("config.db");
        if(!db.open()){qDebug()<<"db was not opened";}
        QSqlQuery query(db);
        QString qStr = "SELECT * FROM gammaChambers WHERE name='%1'";
        QString qStr1 = qStr.arg(nameCh);
        if(!query.exec(qStr1)){qDebug()<<"unable execute query SELECT FROM gammaChambers WHERE name='%1";}
        if(query.first())
        {
            ui->lineEdit->setText(query.value("name").toString());
            ui->lineEdit_2->setText(query.value("type").toString());
            ui->lineEdit_3->setText(query.value("volume").toString());
            ui->lineEdit_4->setText(QString::number(query.value("sensibilityMAED").toDouble()/1000000,'f',5));

            ui->lineEdit_5->setText(query.value("voltage").toString());
            ui->lineEdit_6->setText(query.value("factoryNumber").toString());
            ui->lineEdit_7->setText(query.value("factoryDate").toString());
            ui->lineEdit_8->setText(query.value("verificationDate").toString());
            ui->lineEdit_9->setText(QString::number(query.value("sensibilityMNED").toDouble()/1000000,'f',5));
            ui->lineEdit_10->setText(QString::number(query.value("sensibilityMIED").toDouble()/1000000,'f',5));
            ui->lineEdit_11->setText(QString::number(query.value("sensibilityKERMA").toDouble()/1000000,'f',5));
            ui->lineEdit_12->setText(QString::number(query.value("sensibilityMPD").toDouble()/1000000,'f',5));
            ui->lineEdit_13->setText(QString::number(query.value("sensibilityMED").toDouble()/1000000,'f',5));
            ui->label_19->setText(query.value("sensibilityMAED").toString());
            ui->label_12->setText(query.value("sensibilityMNED").toString());
            ui->label_14->setText(query.value("sensibilityMIED").toString());
            ui->label_16->setText(query.value("sensibilityKERMA").toString());
            ui->label_18->setText(query.value("sensibilityMPD").toString());
            ui->label_22->setText(query.value("sensibilityMED").toString());
        }
    }
}

void VerifyGammaChambers::on_lineEdit_4_textEdited(const QString &arg1) //МАЭД
{
    QString str=arg1;
    str.replace(",",".");  // меняем запятые на точки (точки не выводятся для данных параметров ввода, да и так лучше, чтобы вводилось что-то одно)
    double x=str.toDouble();
    ui->label_19->setText(QString::number(x*1000000,'f',5)); //коэффициент переводит мЗв/нКл  в Зв/Кл
}

void VerifyGammaChambers::on_lineEdit_9_textEdited(const QString &arg1) //МНЭД
{
    QString str=arg1;
    str.replace(",",".");  // меняем запятые на точки (точки не выводятся для данных параметров ввода, да и так лучше, чтобы вводилось что-то одно)
    double x=str.toDouble();
    ui->label_12->setText(QString::number(x*1000000,'f',5)); //коэффициент переводит мЗв/нКл  в Зв/Кл
}

void VerifyGammaChambers::on_lineEdit_10_textEdited(const QString &arg1)    //МИЭД
{
    QString str=arg1;
    str.replace(",",".");  // меняем запятые на точки (точки не выводятся для данных параметров ввода, да и так лучше, чтобы вводилось что-то одно)
    double x=str.toDouble();
    ui->label_14->setText(QString::number(x*1000000,'f',5)); //коэффициент переводит мЗв/нКл  в Зв/Кл
}

void VerifyGammaChambers::on_lineEdit_11_textEdited(const QString &arg1)    //КЕРМА
{
    QString str=arg1;
    str.replace(",",".");  // меняем запятые на точки (точки не выводятся для данных параметров ввода, да и так лучше, чтобы вводилось что-то одно)
    double x=str.toDouble();
    ui->label_16->setText(QString::number(x*1000000,'f',5)); //коэффициент переводит мГр/нКл  в Гр/Кл
}

void VerifyGammaChambers::on_lineEdit_12_textEdited(const QString &arg1)    //МПД
{
    QString str=arg1;
    str.replace(",",".");  // меняем запятые на точки (точки не выводятся для данных параметров ввода, да и так лучше, чтобы вводилось что-то одно)
    double x=str.toDouble();
    ui->label_18->setText(QString::number(x*1000000,'f',5)); //коэффициент переводит мГр/нКл  в Гр/Кл
}

void VerifyGammaChambers::on_lineEdit_13_textEdited(const QString &arg1)
{
    QString str=arg1;
    str.replace(",",".");  // меняем запятые на точки (точки не выводятся для данных параметров ввода, да и так лучше, чтобы вводилось что-то одно)
    double x=str.toDouble();
    ui->label_22->setText(QString::number(x*1000000,'f',5)); //коэффициент переводит мР/нКл  в Р/Кл
}
