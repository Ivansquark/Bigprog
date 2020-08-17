//*********************  Нейтронное излучение - калибровка  ***********************

#include "neutron_verify.h"
#include "ui_neutron_verify.h"

Neutron_verify::Neutron_verify(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Neutron_verify)
{
    ui->setupUi(this);
    init();
}

Neutron_verify::~Neutron_verify()
{    
    delete timer;
    delete startTimer;
    delete bdkn;
    delete msg;
    delete ui;
}

void Neutron_verify::init()
{
    this->setAttribute(Qt::WA_DeleteOnClose); //вызывается деструктор сразу после закрытия окна (а не после закрытия всех пэрэнтов)
    setWindowTitle("Нейтронное излучение - калибровка");
    ui->label->setStyleSheet("font-size:100px; color: red;");
    ui->label_2->setStyleSheet("font-size:50px; color: red;");
    ui->label_3->setStyleSheet("font-size:50px; color: cyan;");
    ui->lineEdit->setValidator(new QDoubleValidator(0.0, 1000.0, 5, ui->lineEdit));
    //только цифры, 5 знаков после запятой
    ui->lineEdit_2->setValidator(new QDoubleValidator(0.0, 1000.0, 5, ui->lineEdit_2));
    //только цифры, 5 знаков после запятой


    timer = new QTimer(this);
    connect(timer,&QTimer::timeout,this,&Neutron_verify::timeOut);
    startTimer = new QTimer(this);
    connect(startTimer,&QTimer::timeout,this,&Neutron_verify::startTimeOut);
}

void Neutron_verify::timeOut()
{
    //------------------------------------Время-------------------------------------------------------
        time++;
        ui->label_3->setText(QString::number(time) + " c"); //отображение времени
    //------------------------------------Мощность дозы-------------------------------------------------------
        I = double(bdkn->IblockCR_Res); //считываем измерение из класса Beta_chamber в Гр/с
        //qDebug()<<beta->MPD();

    //------------------------------------СКО и Среднее-------------------------------------------------------
        Isum+=I;
        avI=(Isum/time);// - средняя величина мощности дозы
        //double avDoseRate = dose*timeCoef;
        ui->label->setText(QString::number(avI,'E'));
        otkl+=I*I; //сумма квадратов случайной величины
        if(time>0)
        {
            double z=sqrt((otkl/time)-avI*avI); //CKO по хитрой формуле: корень из D=(1/N)SUM(1,N,Xi^2)-M^2
            // формула получена на основании того что СКО - это корень из дисперсии
            // Дисперсия - математическое ожидание квадрата отклонения случайной величины от её математического ожидания
            // D=M[X-M(X)]^2 = M(X^2) - M^2(X); - особенное свойство которое везде доказывается
            // M(X^2) - для равновероятных событий является = (1/N)SUM(1,N,doseRate*doseRate)
            // Дисперсия равняется мат ожиданию квадрата случайной величины минус квадрат мат ожидания случайной величины
            // отсюда и получается данная формула: корень из D=(1/N)SUM(1,N,Xi^2)-(среднее)^2

            StandardDeviation=std::abs(z/avI); //под СКО подразумеваем СКО деленное на среднее и умножить на 100% (относительное отклонение от среднего)
            ui->label_4->setText(QString::number(StandardDeviation*100,'f',3)+" %");
        }
}


void Neutron_verify::getBlockBDKN(Block_BDKN* b)
{
    bdkn=b;
    ui->label_5->setText(bdkn->getBlockName());
    MesParam = bdkn->getMesParam();
    MesDiap = bdkn->getMesDiap();
    chamberName = bdkn->getBlockName();
    if(MesParam==0)
    {
        ui->label_9->setText("Плотность Потока");
        ui->comboBox->addItem("имп/(кв.см*с)");
        ui->comboBox->addItem("имп/(кв.м*с)");
        ui->label_6->setText("Значение ПП");
        ui->label_7->setText("Чувствительность, (имп/с)/(имп/(кв.см*с))");
    }
    else if(MesParam==1)
    {
        ui->label_9->setText("Мощность Поглощенной Дозы");
        ui->comboBox->addItem("Гр/с");
        ui->comboBox->addItem("мГр/с");
        ui->comboBox->addItem("мкГр/с");
        ui->comboBox->addItem("Гр/ч");
        ui->comboBox->addItem("мГр/ч");
        ui->comboBox->addItem("мкГр/ч");
        ui->label_6->setText("Значение МПД");
        ui->label_7->setText("Чувствительность, (имп/с)/(Гр/с)");
    }
    else if(MesParam==2)
    {
        ui->label_9->setText("Мощность Амбиентного Эквивалента Дозы");
        ui->comboBox->addItem("Зв/с");
        ui->comboBox->addItem("мЗв/с");
        ui->comboBox->addItem("мкЗв/с");
        ui->comboBox->addItem("Зв/ч");
        ui->comboBox->addItem("мЗв/ч");
        ui->comboBox->addItem("мкЗв/ч");
        ui->label_6->setText("Значение МАЭД");
        ui->label_7->setText("Чувствительность, (имп/с)/(Зв/с)");

    }
    if(MesDiap==0)
    {
        ui->label_8->setText("Без фильтра");

    }
    if(MesDiap==1)
    {
        ui->label_8->setText("С кадмиевым фильтром");

    }
    QString msgStr = "Введите значение параметра %1 \n"
                     "в даннной точке измерения и нажмите старт\n\n"
                     "При нажатии кнопки стоп выведется значение чувствительности,"
                     " которое необходимо записать";
    QString msgStr1 = msgStr.arg(ui->label_9->text());
    msg = new QMessageBox(this);
    msg->setWindowTitle("Внимание");
    msg->setText(msgStr1);
    msg->exec();
    //QMessageBox::information(this,"Внимание",msgStr1,QMessageBox::Ok);
}

void Neutron_verify::on_pushButton_clicked()//назад
{
    close();
}

void Neutron_verify::on_pushButton_2_clicked() // кнопка старт
{
    ui->pushButton_2->setEnabled(false);
    ui->pushButton->setEnabled(false);
    ui->pushButton_2->setVisible(false);
    ui->pushButton->setVisible(false);

    startTimer->setSingleShot(true);
    startTimer->start(1000);
    Isum=0;
   //avI=0;
    time=0;
    otkl=0;
    ui->label_3->setText("ПРОГРЕВ!");
}

void Neutron_verify::startTimeOut()
{
    timer->start(1000);
    //avI=0;
}


void Neutron_verify::on_pushButton_3_clicked() // кнопка стoп
{
    ui->pushButton_2->setEnabled(true);
    ui->pushButton->setEnabled(true);
    ui->pushButton_2->setVisible(true);
    ui->pushButton->setVisible(true);

    timer->stop();
    if(ui->label->text()=="Скорость счета"||ui->lineEdit->text()==""){ui->lineEdit_2->setText("");}
    else
    {
        double I = avI;
        QString str=ui->lineEdit->text(); //мощность дозы или плотность потока в переменную
        str.replace(",",".");  // меняем запятые на точки, чтобы корректно преобразовывался в double
        double doseRate = str.toDouble();
        double sens=0;
        if(MesParam==0) //плотность потока (можно и не разделять т.к. получается что расчитывается одинаково)
        {
            switch (ui->comboBox->currentIndex()) //0-Гр/с 1-мГр/с 2-Гр/ч 3-мГр/ч или 0-имп/(кв.см*с) 1-имп/(кв.м*с)
            {
                case 0: sens=I/doseRate;break; // (имп/с)/((имп/c)/(кв.см*с))
                case 1: sens=(1E3)*I/doseRate;break;//(имп/с)/((имп/c)/(кв.м*с/1000))=(имп/с)/((имп/c)/(кв.см*с)) =>*(1000)
            default: break;
            }
        }
        else             //МПД и МАЭД
        {
            switch (ui->comboBox->currentIndex()) //0-Гр/с 1-мГр/с 2-Гр/ч 3-мГр/ч или 0-имп/(кв.см*с) 1-имп/(кв.м*с)
            {
                case 0: sens=I/doseRate;break; // (имп/с)/(Гр/с)= 1/Гр/с
                case 1: sens=(1E3)*I/doseRate;break; // (имп/с)/(мГр/с)=1/Гр/с *(1000)
                case 2: sens=(1E6)*I/doseRate;break; // (имп/с)/(мкГр/ч)=1/Гр/с  *(1000000)
                case 3: sens=(3.6E3)*I/doseRate;break; // (имп/с)/(Гр/ч)=1/Гр/с *(3600)
                case 4: sens=(3.6E6)*I/doseRate;break; // (имп/с)/(мГр/ч)=1/Гр/с *(3600000)
                case 5: sens=(3.6E9)*I/doseRate;break; // (имп/с)/(мкГр/ч)=1/Гр/с *(3600000000)
            default: break;
            }
        }
        QString sensibility = QString::number(sens);
        ui->lineEdit_2->setText(sensibility);
    }
}

void Neutron_verify::errorConnectionSlot()
{
    QMessageBox::warning(this, "Ошибка","Ошибка в подключении");
    delete msg;
    msg = nullptr;
    close();
}

void Neutron_verify::on_pushButton_4_clicked() //кнопка записать
{
    if (ui->lineEdit_2->text()==""){QMessageBox::warning(this,"Внимание","Не задано значение",QMessageBox::Ok);}
    else
    {
        QString sensibility = ui->lineEdit_2->text(); // Чувствительность в Зависимости от ComboBox-a
        if(QSqlDatabase::contains("myDB"))
        {
            QSqlDatabase db = QSqlDatabase::database("myDB");
            db.setDatabaseName("config.db");
            if(!db.open()){qDebug()<<"db was not opened";}
            QSqlQuery query(db);
            QString qStr="";
            if (MesParam==0&&MesDiap==0){qStr="UPDATE neutronBlocks SET sensibilityPPwithoutCd='%1' WHERE name='%2'";}
            else if (MesParam==0&&MesDiap==1){qStr="UPDATE neutronBlocks SET sensibilityPPwithCd='%1' WHERE name='%2'";}
            else if (MesParam==1&&MesDiap==0){qStr="UPDATE neutronBlocks SET sensibilityMPDwithoutCd='%1' WHERE name='%2'";}
            else if (MesParam==1&&MesDiap==1){qStr="UPDATE neutronBlocks SET sensibilityMPDwithCd='%1' WHERE name='%2'";}
            else if (MesParam==2&&MesDiap==0){qStr="UPDATE neutronBlocks SET sensibilityMAEDwithoutCd='%1' WHERE name='%2'";}
            else if (MesParam==2&&MesDiap==1){qStr="UPDATE neutronBlocks SET sensibilityMAEDwithCd='%1' WHERE name='%2'";}

            QString qStr1=qStr.arg(ui->lineEdit_2->text()).arg(chamberName);
            qDebug()<<chamberName;
            if(!query.exec(qStr1)){qDebug()<<"unable execute query UPDATE gammaBlock";db.lastError().text();}
            db.close();
        }
        QMessageBox::warning(this,"Внимание","Значение чувствительности было изменено",QMessageBox::Ok);
    }
}
