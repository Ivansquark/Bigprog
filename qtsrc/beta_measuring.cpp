//***************************  Бета излучение - измерение (настройки)  *************************

#include "beta_measuring.h"
#include "ui_beta_measuring.h"

Beta_measuring::Beta_measuring(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Beta_measuring)
{
    ui->setupUi(this);
    init();
}

Beta_measuring::~Beta_measuring()
{
    delete betaChamber;
    delete chambComp;
    //qDebug()<<"delete chambComp";
    delete ui;
}


void Beta_measuring::init()
{
    setWindowTitle("Бета излучение - настройки измерения");
    setAttribute(Qt::WA_DeleteOnClose);
    int pass=Password::check(); //вызываем статическую функцию класса Password (статическую, чтобы не создавать объекты)
    switch (pass)                   //проверяем 1 - поверитель или 2 - изготовитель
    {
       case 2: ui->pushButton_3->setEnabled(true);ui->pushButton_3->setVisible(true);
            ui->label_8->setText("ИЗГОТОВИТЕЛЬ:");
            break;
       case 1: ui->pushButton_3->setEnabled(true);ui->pushButton_3->setVisible(true);
            ui->label_8->setText("ПОВЕРИТЕЛЬ:");
            break;
       default:ui->pushButton_3->setEnabled(false);ui->pushButton_3->setVisible(false);
            ui->label_8->setText("ОПЕРАТОР:");
            break;
    }

    if(QSqlDatabase::contains("myDB"))
    {
        QSqlDatabase db = QSqlDatabase::database("myDB");
        db.setDatabaseName("config.db");
        if(!db.open()){qDebug()<<"db was not opened";}
        QSqlQuery query(db);
        QString qStr="SELECT *FROM betaChambers";
        if(!query.exec(qStr)){qDebug()<<"unable execute query SELECT *FROM table betaChambers";}
        QList<QString> strList;
        while(query.next())   //выполняем запросы поочереди в цикле (начиная с первого)
        {
            strList.append(query.value("name").toString()+" дата поверки: "+query.value("VerificationDate").toString());
        }
        ui->comboBox->addItems(strList);


        ui->comboBox_3->addItem("Гр/с");
        ui->comboBox_3->addItem("Гр/мин");
        ui->comboBox_3->addItem("Гр/ч");
        ui->comboBox_4->addItem("Чувствительный");
        ui->comboBox_4->addItem("Средний");
        ui->comboBox_4->addItem("Грубый");

        ui->radioButton->setChecked(true);
        ui->comboBox_2->addItem("Ручной");
        ui->comboBox_2->addItem("Автоматический");
        ui->lineEdit->setText("20");
        ui->lineEdit_2->setText("750");
        ui->label_13->setText("99990");
        ui->lineEdit_3->setText("1");
        ui->lineEdit_4->setEnabled(false);
        ui->lineEdit_4->setVisible(false);
        ui->lineEdit_5->setEnabled(false);
        ui->lineEdit_5->setVisible(false);

        ui->lineEdit_5->setValidator(new QDoubleValidator(0.0, 1000.0, 5, ui->lineEdit_4)); //только цифры, 5 знаков после запятой

        ui->lineEdit_6->setEnabled(false);
        ui->lineEdit_6->setVisible(false);
        ui->lineEdit_7->setEnabled(false);
        ui->lineEdit_7->setVisible(false);

        QString queryS1 = "";
        QString queryS = "SELECT * FROM fullName";
        if(!query.exec(queryS))
        {qDebug()<<"unable execute query SELECT";}
        if(query.first())
        {
            ui->label_9->setText(query.value("surname").toString()+
                                 " "+query.value("name").toString()+
                                 " "+query.value("patronymic").toString());
        }
        db.close();
    }    
    elTest=new Electrometer_test(this);
    elTest->testSource(true);//передаем флаг о том что вызываем из окна измерения
    connect(elTest,&Electrometer_test::closeTestAll,this,&Beta_measuring::testFault);
    elTest->setModal(true);
    elTest->show();
}

void Beta_measuring::on_pushButton_clicked() //назад
{
    close();
}



void Beta_measuring::on_pushButton_3_clicked() //поверка
{
    //  В окне поверки проверяем включен эмулятор или прибор
    //  если эмулятор - то выходим
    //  если прибор, то создаем connect на окно поверки
    //
    uint8_t iR=uint8_t(ui->comboBox_4->currentIndex()); //флаг выбора сопротивления обратной связи (0-R3-чувствительный),(1-R2-средний),(2-R1-грубый)
    bool Comp=ui->checkBox->isChecked();              //флаг нажатия клавиши компенсация (компенсация на эмуляторе не настраивается)
    double Temp=ui->lineEdit->text().toDouble();
    double P=ui->lineEdit_2->text().toDouble();
    double CorrF=ui->lineEdit_3->text().toDouble();
    QString chamName=ui->comboBox->currentText();
    //----------------------------------------------------------------------------------------------
    //передаем установленные параметры из данного окна в конструктор объекта класса бета камеры
    //объект класса Beta_chamber наследуется от класса Ichamber, в котором реализован эмулятор
    betaChamber = new Beta_chamber(iR,Comp,Temp,P,CorrF,chamName);
    connect(this,&Beta_measuring::sendToComSig,betaChamber,&Beta_chamber::setVoltageSlot);
    //-----------------------------------------------------------------------------------------------

    Beta_verify* betaVer = new Beta_verify(this);
    betaVer->getBetaChamber(betaChamber);
    betaVer->show();

    betaVer->setModal(true);

    //connect(прибор,сигнал,betaVer,slot)
    //----------  Управление электрометром: подача высокого, упраление реле резистор отвечающего за диапазон ------------
       QString str=chamName;
       int x=str.indexOf(" ");
       chamName=str.left(x);
       if(QSqlDatabase::contains("myDB"))
       {
           QSqlDatabase db = QSqlDatabase::database("myDB");
           db.setDatabaseName("config.db");
           if(!db.open()){qDebug()<<"db was not opened";}
           QSqlQuery query(db);
           QString qStr = "SELECT * FROM betaChambers WHERE name='%1'";
           QString qStr1 = qStr.arg(chamName);
           if(!query.exec(qStr1)){qDebug()<<"unable execute query SELECT FROM betaChambers WHERE name='%1";}
           if(query.first())   //выполняем запросы поочереди в цикле (начиная с первого)
           {
               voltage = query.value("voltage").toInt(); //узнаем заданное напряжение
           }
           qStr = "SELECT * FROM betaChambers WHERE name='%1'";
           db.close();
       }

           QByteArray byteVolt;
           voltHex volthex;
           byteVolt=volthex.intVolt(voltage); // - через отдельный класс переводим напряжение в цифрах в hex код для посылки в СОМ порт (7х8х)
           //qDebug()<<QString::number(byteVolt[0],16)<<QString::number(byteVolt[1],16);
           //qDebug()<<voltage<<" "<<byteVolt.toHex();
           emit sendToComSig(byteVolt);
           //emit sendToComSig(byteVolt);
}
//*******************************************************************************************
void Beta_measuring::on_pushButton_2_clicked() //кнопка измерение
{
    uint8_t iR=uint8_t(ui->comboBox_4->currentIndex()); //флаг выбора сопротивления обратной связи (0-R3-чувствительный),(1-R2-средний),(2-R1-грубый)
    bool Comp=ui->checkBox->isChecked();              //флаг нажатия клавиши компенсация (компенсация на эмуляторе не настраивается)
    double Temp=ui->lineEdit->text().toDouble();
    double P=ui->lineEdit_2->text().toDouble();
    double CorrF=ui->lineEdit_3->text().toDouble();
    QString chamName=ui->comboBox->currentText();

    //----------------------------------------------------------------------------------------------
    //передаем установленные параметры из данного окна в конструктор объекта класса бета камеры
    //объект класса Beta_chamber наследуется от класса Ichamber, в котором реализован эмулятор
    betaChamber = new Beta_chamber(iR,Comp,Temp,P,CorrF,chamName);
    connect(this,&Beta_measuring::sendToComSig,betaChamber,&Beta_chamber::setVoltageSlot);
    //-----------------------------------------------------------------------------------------------

    QString Dimension=ui->comboBox_3->currentText(); //размерность измеряемой величины
    const int DimensionForTime=ui->comboBox_3->currentIndex(); //временнАя размерность: (0 - 1/с), (1 - 1/мин), (2 - 1/ч) - нужна для учета коэффициента
    bool Regim=ui->comboBox_2->currentIndex();   // режим (0 - ручной), (1 - автоматический).
    const uint TimesAutomatic=ui->lineEdit_6->text().toUInt(); //количество измерений при автоматическом режиме
    const uint TimeAutomatic=ui->lineEdit_7->text().toUInt(); //время автоматического измерения

    uint8_t Threashold=0;            //создаем и инициализируем переменную отвечающую за порог
    if(ui->radioButton->isChecked()) // нет порога
    { Threashold=0; }
    else if(ui->radioButton_2->isChecked()) //по времени
           { Threashold=1; }
           else if(ui->radioButton_3->isChecked()) //по дозе
                  { Threashold=2; }

    const uint TimeThreashold = ui->lineEdit_4->text().toUInt(); // количество секунд в пороге по времени

    QString str=ui->lineEdit_5->text(); //считываем порог по дозе в переменную
    str.replace(",",".");  // меняем запятые на точки, чтобы корректно преобразовывался в double
    const double DoseThreashold=str.toDouble(); // доза при пороге по дозе
    //---------------------------------------------------------------------------------------------------
    //передаем эти параметры в конструктор вновь создаваемого объекта класса окна измерения (бета излучения):
    Beta_measuring_measure* betaMesMes = new Beta_measuring_measure(this,Dimension,DimensionForTime,Regim,
                                                                    TimesAutomatic,TimeAutomatic,Threashold,
                                                                    TimeThreashold,DoseThreashold);
    //---------------------------------------------------------------------------------------------------
    betaMesMes->setModal(true);
    betaMesMes->Beta(betaChamber); // вызываем функцию и передаем туда указатель на текущую объект бета камеры
    betaMesMes->show();
//--------------------------------------------------------------------------------------------------------------------

 //----------  Управление электрометром: подача высокого, упраление реле резистор отвечающего за диапазон ------------
    str=chamName;
    int x=str.indexOf(" ");
    chamName=str.left(x);
    if(QSqlDatabase::contains("myDB"))
    {
        QSqlDatabase db = QSqlDatabase::database("myDB");
        db.setDatabaseName("config.db");
        if(!db.open()){qDebug()<<"db was not opened";}
        QSqlQuery query(db);
        QString qStr = "SELECT * FROM betaChambers WHERE name='%1'";
        QString qStr1 = qStr.arg(chamName);
        if(!query.exec(qStr1)){qDebug()<<"unable execute query SELECT FROM betaChambers WHERE name='%1";}
        if(query.first())   //выполняем запросы поочереди в цикле (начиная с первого)
        {
            voltage = query.value("voltage").toInt();
        }
        db.close();
    }

        QByteArray byteVolt;
        voltHex volthex;

        byteVolt=volthex.intVolt(voltage); // - через отдельный класс переводим напряжение в цифрах в hex код для посылки в СОМ порт (7х8х)
        //qDebug()<<QString::number(byteVolt[0],16)<<QString::number(byteVolt[1],16);
        //qDebug()<<voltage<<" "<<byteVolt.toHex();
        emit sendToComSig(byteVolt);
}


//****************************************************************************************************************

void Beta_measuring::on_lineEdit_2_textEdited(const QString &arg1) //изменение значений давления в мм.рт.ст
{
    const QString str=arg1;
    const double P=arg1.toDouble();
    const double mm_in_Pa=P*133.32;
    ui->label_13->setText(QString::number(mm_in_Pa,'f',0));
}

void Beta_measuring::on_comboBox_2_currentIndexChanged(int index) //ручной, автоматический
{
    if(index==0)//ручной
    {
        ui->lineEdit_6->setEnabled(false); //делаем невидимыми настройки авто измерения
        ui->lineEdit_6->setVisible(false);
        ui->lineEdit_7->setEnabled(false);
        ui->lineEdit_7->setVisible(false);
        ui->label_7->setVisible(false);
        ui->label_11->setVisible(false);

        ui->radioButton->setChecked(true);    //делаем возможной установку порогов
        ui->radioButton_2->setEnabled(true);
        ui->radioButton_3->setEnabled(true);

    }
    if(index==1)//автоматический
    {
        ui->lineEdit_6->setEnabled(true); //делаем видимыми настройки авто измерения
        ui->lineEdit_6->setVisible(true);
        ui->lineEdit_7->setEnabled(true);
        ui->lineEdit_7->setVisible(true);
        ui->label_7->setVisible(true);
        ui->label_11->setVisible(true);
        ui->radioButton->setChecked(true);    //делаем невозможной установку порогов
        ui->radioButton_2->setEnabled(false);
        ui->radioButton_3->setEnabled(false);
        ui->lineEdit_4->setVisible(false);
        ui->lineEdit_5->setVisible(false);
    }
}

void Beta_measuring::on_radioButton_2_clicked() //порог по времени
{
    ui->lineEdit_4->setEnabled(true);
    ui->lineEdit_4->setVisible(true);
    ui->lineEdit_5->setEnabled(false);
    ui->lineEdit_5->setVisible(false);
}

void Beta_measuring::on_radioButton_3_clicked() //порог по дозе
{
    ui->lineEdit_4->setEnabled(false);
    ui->lineEdit_4->setVisible(false);
    ui->lineEdit_5->setEnabled(true);
    ui->lineEdit_5->setVisible(true);
}

void Beta_measuring::on_radioButton_clicked() //порог нет
{
    ui->lineEdit_4->setEnabled(false);
    ui->lineEdit_4->setVisible(false);
    ui->lineEdit_5->setEnabled(false);
    ui->lineEdit_5->setVisible(false);
}


void Beta_measuring::testFault() //прием сигнала о непрохождении теста
{
    close();
}

void Beta_measuring::on_pushButton_4_clicked() //кнопка компенсация
{
    QString chamberName = ui->comboBox->currentText();
    int x=chamberName.indexOf(" ");
    chamberName=chamberName.left(x);
    if(QSqlDatabase::contains("myDB"))
    {
        QSqlDatabase db = QSqlDatabase::database("myDB");
        db.setDatabaseName("config.db");
        if(!db.open()){qDebug()<<"db was not opened";}
        QSqlQuery query(db);
        QString qStr = "SELECT * FROM betaChambers WHERE name='%1'";
        QString qStr1 = qStr.arg(chamberName);
        if(!query.exec(qStr1)){qDebug()<<"unable execute query SELECT FROM betaChambers WHERE name='%1";}
        if(query.first())   //выполняем запросы поочереди в цикле (начиная с первого)
        {
            voltage = query.value("voltage").toInt();
        }
        db.close();
    }
    chambComp = new ChamberCompensation(this,voltage); //создадим объект и передадим ему название камеры
    connect(chambComp,&ChamberCompensation::compensationFlagSig,this,&Beta_measuring::compensationFlagSlot);//коннект на флаг компенсации
    chambComp->setModal(true);
    chambComp->show();
}

void Beta_measuring::on_comboBox_currentIndexChanged(int index) //смена камеры
{
    compensation=false; //скидываем флаг компенсации чтобы одна и таже компенсация не была для разных камер
    if(index==0){}; //чтоб не ругался
}

void Beta_measuring::compensationFlagSlot() // слот приема сигнала о проведенной компенсации
{
    compensation=true;
}

void Beta_measuring::on_checkBox_clicked()//чекбокс учесть компенсацию
{
    if(compensation)
    {
        if(ui->checkBox->isChecked())
        {
            ui->checkBox->setChecked(true);
        }
        else ui->checkBox->setChecked(false);
    }
    else
    {
        ui->checkBox->setChecked(false);
        QMessageBox::warning(this, "Ошибка","Не была проведена компенсация");
    }
}
