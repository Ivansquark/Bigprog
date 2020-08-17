//*************************************** Настройки поверителя - Бета камеры ************************************
#include "verifybetachambers.h"
#include "ui_verifybetachambers.h"

VerifyBetaChambers::VerifyBetaChambers(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VerifyBetaChambers)
{
    ui->setupUi(this);
    init();
}

VerifyBetaChambers::~VerifyBetaChambers()
{
    delete ui;
}

void VerifyBetaChambers::setBut(int x)  //устанавливаем флаг о редактировании=1 или добавлении=0
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

void VerifyBetaChambers::setNameCh(QString str)
{
    nameCh=str;
    if(ButStatus==1)// правка (по имени камеры находим ее в БД и заполняем поля параметрами камеры)
    {
        QSqlDatabase db=QSqlDatabase::database("myDB");
        db.setDatabaseName("config.db");
        if(!db.open()){qDebug()<<"db was not opened";}
        QSqlQuery query(db);
        QString qStr = "SELECT * FROM betaChambers WHERE name='%1'";
        QString qStr1 = qStr.arg(nameCh);
        if(!query.exec(qStr1)){qDebug()<<"unable execute query SELECT FROM betaChambers WHERE name='%1";}
        if(query.first())
        {
            ui->lineEdit->setText(query.value("name").toString());
            ui->lineEdit_2->setText(query.value("diameter").toString());
            ui->lineEdit_3->setText(query.value("elSpace").toString());
            ui->label_5->setText(query.value("sensibility").toString());
            ui->lineEdit_4->setText(QString::number(query.value("sensibility").toDouble()/1000000,'f',5)); //устанавливаем пересчитанное значение из чувствительности Гр/Кл
            ui->comboBox->setCurrentText(query.value("sensibilityDimension").toString());
            ui->lineEdit_5->setText(query.value("voltage").toString());
            ui->lineEdit_6->setText(query.value("factoryNumber").toString());
            ui->lineEdit_7->setText(query.value("factoryDate").toString());
            ui->lineEdit_8->setText(query.value("verificationDate").toString());
        }
    }
}

void VerifyBetaChambers::init()
{
    ui->comboBox->addItem("мГр/нКл");
    QRegExp expName;
    expName.setPattern(QString("[а-яА-Я]+[-0-9]{10}"));  //ограничение названия Русскими буквами дефисом и цифрами
    QRegExpValidator *rx=new QRegExpValidator(expName,this);
    ui->lineEdit->setValidator(rx);
    ui->lineEdit_2->setValidator(new QIntValidator(10,50,ui->lineEdit_2));
    ui->lineEdit_3->setValidator(new QDoubleValidator(0.1,1.0,4,ui->lineEdit_3));
    ui->lineEdit_4->setValidator(new QDoubleValidator(0.0, 1000.0, 5, ui->lineEdit_4)); //только цифры, 5 знаков после запятой
    ui->lineEdit_5->setValidator(new QIntValidator(-400,400,ui->lineEdit_5)); //только int
    ui->lineEdit_7->setInputMask("00.00.0000");
    ui->lineEdit_8->setInputMask("00.00.0000"); // для даты
    setWindowTitle("Камеры бета излучения");
}

void VerifyBetaChambers::on_pushButton_2_clicked() //назад
{
    close();
}

void VerifyBetaChambers::on_lineEdit_4_textEdited(const QString &arg1) // набор текста в строке чувствительность
{
    QString str=arg1;
    str.replace(",",".");  // меняем запятые на точки (точки не выводятся для данных параметров ввода, да и так лучше, чтобы вводилось что-то одно)
    double x=str.toDouble();
    ui->label_5->setText(QString::number(x*1000000,'f',5)); //коэффициент переводит мГр/нКл  в Гр/Кл
}

void VerifyBetaChambers::on_pushButton_clicked() //Ок
{
    if(ButStatus==0)  //добавляем камеру (только в режиме изготовителя)
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
            QString qStr="INSERT INTO betaChambers(name, diameter, elSpace, sensibility,"
                                                    "sensibilityDimension, voltage, factoryNumber,"
                                                    "factoryDate, verificationDate) "
                         "VALUES ('%1','%2','%3','%4','%5','%6','%7','%8','%9')";
            QString qStr1=qStr.arg(ui->lineEdit->text()).                           //запрос на вставку новой строки в БД
                               arg(ui->lineEdit_2->text()).                         //новые параметры берем из lineEdit-ов
                               arg(ui->lineEdit_3->text()).
                               arg(ui->label_5->text()).
                               arg(ui->comboBox->currentText()).
                               arg(ui->lineEdit_5->text()).
                               arg(ui->lineEdit_6->text()).
                               arg(ui->lineEdit_7->text()).
                               arg(ui->lineEdit_8->text());
            if(!query.exec(qStr1)){qDebug()<<"unable execute query INSERT INTO table betaChambers"<<query.lastError().text();}
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
    else if(ButStatus==1) //правка
    {
        bool emptyFlag=false;                                       //объявляем и инициализируем флаг проверки на пустые строки
        QList<QLineEdit*> lst = this->findChildren<QLineEdit*>();   //все lineEdit-ы кладем в QList
        for(QLineEdit* i:lst)                                       //в цикле проверяем на пустоту
        {
            if(i->text().isEmpty()){emptyFlag=true;}                //если есть пустые выставляем флаг
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
            QString qStr="UPDATE betaChambers SET sensibility='%1',sensibilityDimension='%2',"
                                                 "voltage='%3', verificationDate='%4' WHERE name='%5'";

            QString qStr1=qStr.arg(ui->label_5->text()).
                               arg(ui->comboBox->currentText()).
                               arg(ui->lineEdit_5->text()).
                               arg(ui->lineEdit_8->text()).
                               arg(nameCh);
            if(!query.exec(qStr1)){qDebug()<<"unable execute query UPDATE betaChambers";}
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
}
