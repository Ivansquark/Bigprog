//****************************  Нейтронное излучение - настройки измерения *************************

#include "neutron_measuring.h"
#include "ui_neutron_measuring.h"

Neutron_measuring::Neutron_measuring(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Neutron_measuring)
{
    ui->setupUi(this);
    init();
}

Neutron_measuring::~Neutron_measuring()
{
    qDebug()<<"~Neutron_measuring(), bdkn";
    //delete bdkn;
    qDebug()<<"AAAA~Neutron_measuring(), bdkn";
    delete ui;
}

void Neutron_measuring::init()
{
    RS485::BDGorBDKN = 1;
    this->setAttribute(Qt::WA_DeleteOnClose); //вызывается деструктор сразу после закрытия окна (а не после закрытия всех пэрэнтов)
    setWindowTitle("Нейтронное излучение - настройки измерения");
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
        QString qStr="SELECT *FROM neutronBlocks";
        if(!query.exec(qStr)){qDebug()<<"unable execute query SELECT *FROM table neutronBlocks";}
        QList<QString> strList;
        while(query.next())   //выполняем запросы поочереди в цикле (начиная с первого)
        {
            strList.append(query.value("name").toString()+" дата поверки: "+query.value("VerificationDate").toString());
        }
        ui->comboBox->addItems(strList);

        ui->comboBox_4->addItem("Плотность Потока");
        ui->comboBox_4->addItem("Мощность Поглощенной Дозы");
        ui->comboBox_4->addItem("Мощность Амбиентного Эквивалента Дозы");

        ui->comboBox_3->clear();
        ui->comboBox_3->addItem("част/(с*кв.см)");
        ui->comboBox_3->addItem("част/(мин*кв.см)");
        ui->comboBox_3->addItem("част/(ч*кв.см)");

        ui->comboBox_5->addItem("Нижний (без кадмиевого фильтра)");
        ui->comboBox_5->addItem("Верхний (с кадмиевым фильтром)");

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

        ui->lineEdit->setValidator(new QDoubleValidator(0.0, 1000.0, 5, ui->lineEdit_4)); //только цифры, 5 знаков после запятой
        ui->lineEdit_2->setValidator(new QDoubleValidator(0.0, 1000.0, 5, ui->lineEdit_4)); //только цифры, 5 знаков после запятой
        ui->lineEdit_3->setValidator(new QDoubleValidator(0.0, 1000.0, 5, ui->lineEdit_4)); //только цифры, 5 знаков после запятой

        ui->lineEdit_5->setValidator(new QDoubleValidator(0.0, 1000.0, 5, ui->lineEdit_4)); //только цифры, 5 знаков после запятой

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
    ui->comboBox_3->setStyleSheet("font-size: 12px;");
}

void Neutron_measuring::on_pushButton_clicked() //назад
{
    close();
}


void Neutron_measuring::on_pushButton_3_clicked() //поверка
{
    float Temp=ui->lineEdit->text().toFloat();
    float P = ui->lineEdit_2->text().toFloat();
    float CorrF=ui->lineEdit_3->text().toFloat();

    QString blockName=ui->comboBox->currentText();
    int x=blockName.indexOf(" ");
    blockName=blockName.left(x); //имя блока (Америций или Цезий или Кобальт)

    quint8 mesParam = quint8(ui->comboBox_4->currentIndex()); //параметр измерения (0-ПП, 1-МПД, 2-МАЭД)
    quint8 diapParam = quint8(ui->comboBox_5->currentIndex()); //диапазон измерения (0-нижний, 1-верхний)

    bdkn = new Block_BDKN(Temp,P,CorrF,blockName,mesParam,diapParam);//создается объект блока БДГ (в него передаются соответствующие ему параметры)
    connect(bdkn,&Block_BDKN::errorSig,this,&Neutron_measuring::errorConnectionSlot);

    Neutron_verify* neutronVer = new Neutron_verify(this);
    connect(this,&Neutron_measuring::errorConnectionSignal,neutronVer,&Neutron_verify::errorConnectionSlot);
    neutronVer->getBlockBDKN(bdkn);
    neutronVer->setModal(true);
    neutronVer->show();
}

void Neutron_measuring::on_pushButton_2_clicked() //измерение
{
    float Temp=ui->lineEdit->text().toFloat();
    float P = ui->lineEdit_2->text().toFloat();
    float CorrF=ui->lineEdit_3->text().toFloat();

    QString blockName=ui->comboBox->currentText();
    int x=blockName.indexOf(" ");
    blockName=blockName.left(x); //имя блока (Америций или Цезий или Кобальт)

    quint8 mesParam = quint8(ui->comboBox_4->currentIndex()); //параметр измерения (0-ПП, 1-МПД, 2-МАЭД)
    quint8 diapParam = quint8(ui->comboBox_5->currentIndex()); //диапазон измерения (0-нижний, 1-верхний)

    bdkn = new Block_BDKN(Temp,P,CorrF,blockName,mesParam,diapParam);//создается объект блока БДГ (в него передаются соответствующие ему параметры)
    connect(bdkn,&Block_BDKN::errorSig,this,&Neutron_measuring::errorConnectionSlot);

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


    Neutron_measuring_measure* neutronMesMes = new Neutron_measuring_measure(this,
                                                                             Dimension,
                                                                             DimensionForTime,
                                                                             Regime,
                                                                             TimesAutomatic,
                                                                             TimeAutomatic,
                                                                             Threashold,
                                                                             TimeThreashold,
                                                                             DoseThreashold);
    connect(this,&Neutron_measuring::errorConnectionSignal,neutronMesMes,&Neutron_measuring_measure::errorConnectionSlot);
    neutronMesMes->setModal(true);
    neutronMesMes->neutron(bdkn);
    neutronMesMes->show();
}

void Neutron_measuring::on_comboBox_2_currentIndexChanged(int index)//режим (0-ручной, 1-автоматический)
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

void Neutron_measuring::on_radioButton_2_clicked() //порог по времени
{
    ui->lineEdit_4->setEnabled(true);
    ui->lineEdit_4->setVisible(true);
    ui->lineEdit_5->setEnabled(false);
    ui->lineEdit_5->setVisible(false);
}

void Neutron_measuring::on_radioButton_3_clicked() //порог по дозе
{
    ui->lineEdit_4->setEnabled(false);
    ui->lineEdit_4->setVisible(false);
    ui->lineEdit_5->setEnabled(true);
    ui->lineEdit_5->setVisible(true);
}

void Neutron_measuring::on_radioButton_clicked() //порог нет
{
    ui->lineEdit_4->setEnabled(false);
    ui->lineEdit_4->setVisible(false);
    ui->lineEdit_5->setEnabled(false);
    ui->lineEdit_5->setVisible(false);
}

void Neutron_measuring::on_comboBox_4_currentIndexChanged(int index) //измеряемая величина (0-ПП, 1-МПД, 2-МАЭД)
{
    switch (index)
    {
    case 0:
        ui->comboBox_3->clear();
        ui->comboBox_3->addItem("част/(с*кв.см)");
        ui->comboBox_3->addItem("част/(мин*кв.см)");
        ui->comboBox_3->addItem("част/(ч*кв.см)");
        ui->radioButton_3->setText("по флюенсу, част/см.кв.");
        break;
    case 1:
        ui->comboBox_3->clear();
        ui->comboBox_3->addItem("Гр/с");
        ui->comboBox_3->addItem("Гр/мин");
        ui->comboBox_3->addItem("Гр/ч");
        ui->radioButton_3->setText("по дозе, мГр");
        break;
    case 2:
        ui->comboBox_3->clear();
        ui->comboBox_3->addItem("Зв/с");
        ui->comboBox_3->addItem("Зв/мин");
        ui->comboBox_3->addItem("Зв/ч");
        ui->radioButton_3->setText("по дозе, мЗв");
        break;
    default: break;
    }
}

void Neutron_measuring::on_lineEdit_2_textEdited(const QString &arg1) // ввод давления
{
    const QString str=arg1;
    const double P=arg1.toDouble();
    const double mm_in_Pa=P*133.32;
    ui->label_15->setText(QString::number(mm_in_Pa,'f',0));
}

void Neutron_measuring::errorConnectionSlot()
{
    emit errorConnectionSignal();
}
