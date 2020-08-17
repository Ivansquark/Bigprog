//*************  "Гамма излучение - ионизационные камеры - настройки измерения"  ***********************

#include "gamma_ik_measuring.h"
#include "ui_gamma_ik_measuring.h"

Gamma_IK_measuring::Gamma_IK_measuring(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Gamma_IK_measuring)
{
    ui->setupUi(this);
    init();
}

Gamma_IK_measuring::~Gamma_IK_measuring()
{
    delete ui;
}

void Gamma_IK_measuring::init()
{
    setWindowTitle("Гамма излучение - ионизационные камеры - настройки измерения");
    this->setAttribute(Qt::WA_DeleteOnClose); //вызывается деструктор сразу после закрытия окна (а не после закрытия всех пэрэнтов)

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
        QString qStr="SELECT *FROM gammaChambers";
        if(!query.exec(qStr)){qDebug()<<"unable execute query SELECT *FROM table gammaChambers";}
        QList<QString> strList;
        while(query.next())   //выполняем запросы поочереди в цикле (начиная с первого)
        {
            strList.append(query.value("name").toString()+" дата поверки: "+query.value("VerificationDate").toString());
        }
        ui->comboBox->addItems(strList);

        ui->comboBox_4->addItem("Мощность Амбиентного Эквивалента Дозы");
        ui->comboBox_4->addItem("Мощность Направленного Эквивалента Дозы");
        ui->comboBox_4->addItem("Мощность Индивидуального Эквивалента Дозы");
        ui->comboBox_4->addItem("Мощность КЕРМЫ");
        ui->comboBox_4->addItem("Мощность Поглощенной Дозы");
        ui->comboBox_4->addItem("Мощность Экспозиционной Дозы");

        ui->comboBox_3->clear();
        ui->comboBox_3->addItem("Зв/с");
        ui->comboBox_3->addItem("Зв/мин");
        ui->comboBox_3->addItem("Зв/ч");

        ui->comboBox_5->addItem("Чувствительный");
        ui->comboBox_5->addItem("Средний");
        ui->comboBox_5->addItem("Грубый");

        ui->radioButton->setChecked(true);
        ui->comboBox_2->addItem("Ручной");
        ui->comboBox_2->addItem("Автоматический");
        ui->lineEdit->setText("20");
        ui->lineEdit_2->setText("760");
        ui->label_15->setText("101323");
        ui->lineEdit_3->setText("1");
        ui->lineEdit_4->setEnabled(false);
        ui->lineEdit_4->setVisible(false);
        ui->lineEdit_5->setEnabled(false);
        ui->lineEdit_5->setVisible(false);

        ui->lineEdit->setValidator(new QDoubleValidator(0.0, 1000.0, 5, ui->lineEdit)); //только цифры, 5 знаков после запятой
        ui->lineEdit_2->setValidator(new QDoubleValidator(0.0, 1000.0, 5, ui->lineEdit_2)); //только цифры, 5 знаков после запятой
        ui->lineEdit_3->setValidator(new QDoubleValidator(0.0, 1000.0, 5, ui->lineEdit_3)); //только цифры, 5 знаков после запятой

        ui->lineEdit_5->setValidator(new QDoubleValidator(0.0, 1000.0, 5, ui->lineEdit_5)); //только цифры, 5 знаков после запятой

        ui->lineEdit_6->setEnabled(false);
        ui->lineEdit_6->setVisible(false);
        ui->lineEdit_7->setEnabled(false);
        ui->lineEdit_7->setVisible(false);

        ui->label_12->setVisible(false);
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
    connect(elTest,&Electrometer_test::closeTestAll,this,&Gamma_IK_measuring::testFault);
    elTest->setModal(true);
    elTest->show();
}

void Gamma_IK_measuring::on_pushButton_clicked() //назад
{
    close();
}

void Gamma_IK_measuring::on_pushButton_2_clicked()//измерение
{
    uint8_t iR=uint8_t(ui->comboBox_5->currentIndex()); //флаг выбора сопротивления обратной связи (0-R3-чувствительный),(1-R2-средний),(2-R1-грубый)
    bool Comp=ui->checkBox->isChecked();              //флаг нажатия клавиши компенсация (компенсация на эмуляторе не настраивается)
    double Temp=ui->lineEdit->text().toDouble();
    double P=ui->lineEdit_2->text().toDouble();
    double CorrF=ui->lineEdit_3->text().toDouble();
    QString chamName=ui->comboBox->currentText();
    quint8 mesParam = quint8(ui->comboBox_4->currentIndex()); //параметр измерения (0-МАЭД, 1-МНЭД, 2-МИЭД, 3-КЕРМА, 4-МПД, 5-МЭД)


    //----------------------------------------------------------------------------------------------
    //передаем установленные параметры из данного окна в конструктор объекта класса гамма камеры
    //объект класса GammaIK_chamber наследуется от класса Ichamber, в котором реализован эмулятор
    IKgamma = new GammaIK_chamber(iR,Comp,Temp,P,CorrF,chamName,mesParam);
    //коннект на слот который в находится в родителе GammaIK_chamber - Ichamber
    connect(this,&Gamma_IK_measuring::sendToComSig,IKgamma,&GammaIK_chamber::setVoltageSlot);

    //-------------------------------------------------------------------------------------

    QString Dimension=ui->comboBox_3->currentText(); //размерность измеряемой величины
    const int DimensionForTime=ui->comboBox_3->currentIndex(); //временнАя размерность: (0 - 1/с), (1 - 1/мин), (2 - 1/ч) - нужна для учета коэффициента
    bool Regime=ui->comboBox_2->currentIndex();   // режим (0 - ручной), (1 - автоматический).
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

    Gamma_IK_measuring_measure* gamma_IK_mesMes = new Gamma_IK_measuring_measure(this,Dimension,
                                                                                      DimensionForTime,
                                                                                      Regime,
                                                                                      TimesAutomatic,
                                                                                      TimeAutomatic,
                                                                                      Threashold,
                                                                                      TimeThreashold,
                                                                                      DoseThreashold);

    gamma_IK_mesMes->setModal(true);
    gamma_IK_mesMes->GammaIK(IKgamma);
    gamma_IK_mesMes->show();
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
        QString qStr = "SELECT * FROM gammaChambers WHERE name='%1'";
        QString qStr1 = qStr.arg(chamName);
        if(!query.exec(qStr1)){qDebug()<<"unable execute query SELECT FROM gammaChambers WHERE name='%1";}
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

void Gamma_IK_measuring::on_pushButton_3_clicked() // поверка
{
    uint8_t iR=uint8_t(ui->comboBox_5->currentIndex()); //флаг выбора сопротивления обратной связи (0-R3-чувствительный),(1-R2-средний),(2-R1-грубый)
    bool Comp=ui->checkBox->isChecked();              //флаг нажатия клавиши компенсация (компенсация на эмуляторе не настраивается)
    double Temp=ui->lineEdit->text().toDouble();
    double P=ui->lineEdit_2->text().toDouble();
    double CorrF=ui->lineEdit_3->text().toDouble();
    QString chamName=ui->comboBox->currentText();
    quint8 mesParam = quint8(ui->comboBox_4->currentIndex()); //параметр измерения (0-МАЭД, 1-МНЭД, 2-МИЭД, 3-КЕРМА, 4-МПД, 5-МЭД)
    //----------------------------------------------------------------------------------------------
    //передаем установленные параметры из данного окна в конструктор объекта класса гамма камеры
    //объект класса GammaIK_chamber наследуется от класса Ichamber, в котором реализован эмулятор
    IKgamma = new GammaIK_chamber(iR,Comp,Temp,P,CorrF,chamName,mesParam);
    //коннект на слот который в находится в родителе GammaIK_chamber - Ichamber
    connect(this,&Gamma_IK_measuring::sendToComSig,IKgamma,&GammaIK_chamber::setVoltageSlot);
    Gamma_IK_verify* gammaIKver = new Gamma_IK_verify(this);
    gammaIKver->setModal(true);
    gammaIKver->show();
    gammaIKver->getGammaChamber(IKgamma);

    //----------  Управление электрометром: подача высокого, упраление реле резистор отвечающего за диапазон ------------
    QString str=chamName;
    int x=str.indexOf(" ");
    chamName=str.left(x);   //имя камера
    if(QSqlDatabase::contains("myDB"))
    {
        QSqlDatabase db = QSqlDatabase::database("myDB");
        db.setDatabaseName("config.db");
        if(!db.open()){qDebug()<<"db was not opened";}
        QSqlQuery query(db);
        QString qStr = "SELECT * FROM gammaChambers WHERE name='%1'";
        QString qStr1 = qStr.arg(chamName);
        if(!query.exec(qStr1)){qDebug()<<"unable execute query SELECT FROM gammaChambers WHERE name='%1";}
        if(query.first())   //выполняем запросы поочереди в цикле (начиная с первого)
        {
            voltage = query.value("voltage").toInt();
        }
        db.close();
    }
    QByteArray byteVolt;
    voltHex volthex;
    byteVolt=volthex.intVolt(voltage); // - через отдельный класс переводим напряжение в цифрах в hex код для посылки в СОМ порт (7х8х)
    //qDebug()<<voltage;
    //qDebug()<<QString::number(byteVolt[0],16)<<QString::number(byteVolt[1],16);
           //qDebug()<<voltage<<" "<<byteVolt.toHex();

    emit sendToComSig(byteVolt);
}

void Gamma_IK_measuring::on_comboBox_4_currentIndexChanged(int index)
{
    switch (index)
    {
    case 0:
        ui->comboBox_3->clear();
        ui->comboBox_3->addItem("Зв/с");
        ui->comboBox_3->addItem("Зв/мин");
        ui->comboBox_3->addItem("Зв/ч");
        ui->radioButton_3->setText("по дозе, мЗв");
        break;
    case 1:
        ui->comboBox_3->clear();
        ui->comboBox_3->addItem("Зв/с");
        ui->comboBox_3->addItem("Зв/мин");
        ui->comboBox_3->addItem("Зв/ч");
        ui->radioButton_3->setText("по дозе, мЗв");
        break;
    case 2:
        ui->comboBox_3->clear();
        ui->comboBox_3->addItem("Зв/с");
        ui->comboBox_3->addItem("Зв/мин");
        ui->comboBox_3->addItem("Зв/ч");
        ui->radioButton_3->setText("по дозе, мЗв");
        break;
    case 3:
        ui->comboBox_3->clear();
        ui->comboBox_3->addItem("Гр/с");
        ui->comboBox_3->addItem("Гр/мин");
        ui->comboBox_3->addItem("Гр/ч");
        ui->radioButton_3->setText("по дозе, мГр");
        break;
    case 4:
        ui->comboBox_3->clear();
        ui->comboBox_3->addItem("Гр/с");
        ui->comboBox_3->addItem("Гр/мин");
        ui->comboBox_3->addItem("Гр/ч");
        ui->radioButton_3->setText("по дозе, мГр");
        break;
    case 5:
        ui->comboBox_3->clear();
        ui->comboBox_3->addItem("Р/с");
        ui->comboBox_3->addItem("Р/мин");
        ui->comboBox_3->addItem("Р/ч");
        ui->radioButton_3->setText("по дозе, мР");
        break;
    default: break;
    }
}

void Gamma_IK_measuring::on_lineEdit_2_textEdited(const QString &arg1)
{
    const QString str=arg1;
    const double P=arg1.toDouble();
    const double mm_in_Pa=P*133.32;
    ui->label_15->setText(QString::number(mm_in_Pa,'f',0));
}

void Gamma_IK_measuring::on_comboBox_2_currentIndexChanged(int index)
{
    if(index==0)//ручной
    {
        ui->lineEdit_6->setEnabled(false); //делаем невидимыми настройки авто измерения
        ui->lineEdit_6->setVisible(false);
        ui->lineEdit_7->setEnabled(false);
        ui->lineEdit_7->setVisible(false);
        ui->label_7->setVisible(false);
        ui->label_12->setVisible(false);

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
        ui->label_12->setVisible(true);
        ui->radioButton->setChecked(true);    //делаем невозможной установку порогов
        ui->radioButton_2->setEnabled(false);
        ui->radioButton_3->setEnabled(false);
        ui->lineEdit_4->setVisible(false);
        ui->lineEdit_5->setVisible(false);
    }
}

void Gamma_IK_measuring::on_radioButton_2_clicked() //порог по времени
{
    ui->lineEdit_4->setEnabled(true);
    ui->lineEdit_4->setVisible(true);
    ui->lineEdit_5->setEnabled(false);
    ui->lineEdit_5->setVisible(false);
}

void Gamma_IK_measuring::on_radioButton_3_clicked() // порог по дозе
{
    ui->lineEdit_4->setEnabled(false);
    ui->lineEdit_4->setVisible(false);
    ui->lineEdit_5->setEnabled(true);
    ui->lineEdit_5->setVisible(true);
}

void Gamma_IK_measuring::on_radioButton_clicked() // порога нет
{
    ui->lineEdit_4->setEnabled(false);
    ui->lineEdit_4->setVisible(false);
    ui->lineEdit_5->setEnabled(false);
    ui->lineEdit_5->setVisible(false);
}

void Gamma_IK_measuring::testFault() //прием сигнала о том что тест не пройден
{
    close();
}


void Gamma_IK_measuring::on_pushButton_4_clicked() //кнопка вызова расчета компенсации
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
        QString qStr = "SELECT * FROM gammaChambers WHERE name='%1'";
        QString qStr1 = qStr.arg(chamberName);
        if(!query.exec(qStr1)){qDebug()<<"unable execute query SELECT FROM betaChambers WHERE name='%1";}
        if(query.first())   //выполняем запросы поочереди в цикле (начиная с первого)
        {
            voltage = query.value("voltage").toInt();
        }
        db.close();
    }
    qDebug()<<voltage;
    chambComp = new ChamberCompensation(this,voltage); //создадим объект и передадим ему название камеры
    connect(chambComp,&ChamberCompensation::compensationFlagSig,this,&Gamma_IK_measuring::compensationFlagSlot);//коннект на флаг компенсации
    chambComp->setModal(true);
    chambComp->show();
}

void Gamma_IK_measuring::on_comboBox_currentIndexChanged(int index)
{
    compensation=false; //скидываем флаг компенсации чтобы одна и таже компенсация не была для разных камер
    if(index==0){}; //чтоб не ругался на неиспользуемый индекс
}

void Gamma_IK_measuring::compensationFlagSlot() //слот выставляющий компенсацию
{
     compensation=true;
}


void Gamma_IK_measuring::on_checkBox_clicked()
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
