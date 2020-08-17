//*******************  Гамма излучение - ионизационные камеры - калибровка  **************************

#include "gamma_ik_verify.h"
#include "ui_gamma_ik_verify.h"

Gamma_IK_verify::Gamma_IK_verify(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Gamma_IK_verify)
{
    ui->setupUi(this);
    init();
}

Gamma_IK_verify::~Gamma_IK_verify()
{
    delete timer;
    delete startTimer;
    delete IKgamma;
    delete ui;
}

void Gamma_IK_verify::init()
{
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle("Гамма излучение - ионизационные камеры - калибровка");
    ui->label->setStyleSheet("font-size:100px; color: red;");
    ui->label_2->setStyleSheet("font-size:100px; color: red;");
    ui->label_3->setStyleSheet("font-size:50px; color: cyan;");
    ui->lineEdit->setValidator(new QDoubleValidator(0.0, 1000.0, 5, ui->lineEdit));
    //только цифры, 5 знаков после запятой
    ui->lineEdit_2->setValidator(new QDoubleValidator(0.0, 1000.0, 5, ui->lineEdit_2));
    //только цифры, 5 знаков после запятой

    timer = new QTimer(this);
    connect(timer,&QTimer::timeout,this,&Gamma_IK_verify::timeOut);
    startTimer = new QTimer(this);
    connect(startTimer,&QTimer::timeout,this,&Gamma_IK_verify::startTimeOut);

    /*!< Окно с задержкой выполнения программы > */
    QMessageBox* startMsg = new QMessageBox(QMessageBox::Icon::Information,"","",QMessageBox::NoButton,this,Qt::FramelessWindowHint);
    startMsg->setText("Произодится конфигурирование оборудования");
    startMsg->setStandardButtons(0);
    startMsg->setStyleSheet("background-color: rgb(180,178,177);");
    QTimer::singleShot(4000,startMsg,SLOT(hide()));
    startMsg->exec();
    delete startMsg;
}

void Gamma_IK_verify::timeOut()
{
    //------------------------------------Время-------------------------------------------------------
        time++;
        ui->label_3->setText(QString::number(time)); //отображение времени
    //------------------------------------Мощность дозы-------------------------------------------------------
        I = IKgamma->getIres(); //считываем измерение из класса Beta_chamber в Гр/с
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


void Gamma_IK_verify::getGammaChamber(GammaIK_chamber *beta)
{
    IKgamma = beta;
    ui->label_5->setText(IKgamma->getBlockName());
    chamberName = IKgamma->getBlockName();
    MesParam = IKgamma->getMesParam();
    if(MesParam==0)
    {
        ui->label_8->setText("Мощность Амбиентного Эквивалента Дозы");
        ui->comboBox->addItem("Зв/с");
        ui->comboBox->addItem("мЗв/с");
        ui->comboBox->addItem("мкЗв/с");
        ui->comboBox->addItem("Зв/ч");
        ui->comboBox->addItem("мЗв/ч");
        ui->comboBox->addItem("мкЗв/ч");
        ui->label_6->setText("Значение МАЭД");
        ui->label_7->setText("Чувствительность, Зв/Кл");
    }
    else if(MesParam==1)
    {
        ui->label_8->setText("Мощность Индивидуального Эквивалента Дозы");
        ui->comboBox->addItem("Зв/с");
        ui->comboBox->addItem("мЗв/с");
        ui->comboBox->addItem("мкЗв/с");
        ui->comboBox->addItem("Зв/ч");
        ui->comboBox->addItem("мЗв/ч");
        ui->comboBox->addItem("мкЗв/ч");
        ui->label_6->setText("Значение МИЭД");
        ui->label_7->setText("Чувствительность, Зв/Кл");
    }
    else if(MesParam==2)
    {
        ui->label_8->setText("Мощность Направленного Эквивалента Дозы");
        ui->comboBox->addItem("Зв/с");
        ui->comboBox->addItem("мЗв/с");
        ui->comboBox->addItem("мкЗв/с");
        ui->comboBox->addItem("Зв/ч");
        ui->comboBox->addItem("мЗв/ч");
        ui->comboBox->addItem("мкЗв/ч");
        ui->label_6->setText("Значение МНЭД");
        ui->label_7->setText("Чувствительность, Зв/Кл");
    }
    else if(MesParam==3)
    {
        ui->label_8->setText("Мощность Кермы");
        ui->comboBox->addItem("Гр/с");
        ui->comboBox->addItem("мГр/с");
        ui->comboBox->addItem("мкГр/с");
        ui->comboBox->addItem("Гр/ч");
        ui->comboBox->addItem("мГр/ч");
        ui->comboBox->addItem("мкГр/ч");
        ui->label_6->setText("Значение Мощности КЕРМА");
        ui->label_7->setText("Чувствительность, Гр/Кл");
    }
    else if(MesParam==4)
    {
        ui->label_8->setText("Мощность Поглощенной Дозы");
        ui->comboBox->addItem("Гр/с");
        ui->comboBox->addItem("мГр/с");
        ui->comboBox->addItem("мкГр/с");
        ui->comboBox->addItem("Гр/ч");
        ui->comboBox->addItem("мГр/ч");
        ui->comboBox->addItem("мкГр/ч");
        ui->label_6->setText("Значение МПД");
        ui->label_7->setText("Чувствительность, Гр/Кл");
    }
    else if(MesParam==5)
    {
        ui->label_8->setText("Мощность Экспозиционной Дозы");
        ui->comboBox->addItem("Р/с");
        ui->comboBox->addItem("мР/с");
        ui->comboBox->addItem("мкР/с");
        ui->comboBox->addItem("Р/ч");
        ui->comboBox->addItem("мР/ч");
        ui->comboBox->addItem("мкР/ч");
        ui->label_6->setText("Значение МЭД");
        ui->label_7->setText("Чувствительность, Р/Кл");
    }
    QString msgStr = "Введите значение параметра %1 \n"
                     "в даннной точке измерения и нажмите старт\n\n"
                     "При нажатии кнопки стоп выведется значение чувствительности,"
                     " которое необходимо записать";
    QString msgStr1 = msgStr.arg(ui->label_8->text());
    QMessageBox::information(this,"Внимание",msgStr1,QMessageBox::Ok);
}

void Gamma_IK_verify::on_pushButton_clicked() //назад
{

    close();
}

void Gamma_IK_verify::on_pushButton_2_clicked() //кнопка старт
{    
    ui->pushButton_2->setEnabled(false);
    ui->pushButton->setEnabled(false);
    ui->pushButton_2->setVisible(false);
    ui->pushButton->setVisible(false);
    ui->pushButton_3->setEnabled(false);
    startTimer->setSingleShot(true);
    startTimer->start(60000);
    Isum=0;
    time=0;
    otkl=0;
    ui->label_3->setText("ПРОГРЕВ!");
}

void Gamma_IK_verify::startTimeOut()
{
    ui->pushButton_3->setEnabled(true);
    timer->start(1000);
}

void Gamma_IK_verify::on_pushButton_3_clicked() // кнопка стоп
{
    ui->pushButton_2->setEnabled(true);
    ui->pushButton->setEnabled(true);
    ui->pushButton_2->setVisible(true);
    ui->pushButton->setVisible(true);
    timer->stop();
    if(ui->label->text()=="Ток"||ui->lineEdit->text()==""){ui->lineEdit_2->setText("");}
    else
    {
        double I = avI;
        QString str=ui->lineEdit->text(); //считываем порог по дозе в переменную
        str.replace(",",".");  // меняем запятые на точки, чтобы корректно преобразовывался в double
        double doseRate = str.toDouble();
        double sens=0;
        switch (ui->comboBox->currentIndex()) //0-Гр/с 1-мГр/с 2-Гр/ч 3-мГр/ч
        {
            case 0: sens=std::abs(doseRate/I);break; // (Гр/с)/(Кл/с)=Гр/с
            case 1: sens=std::abs((1/(1E3))*doseRate/I);break; // (мГр*1000/с)/(Кл/с)=Гр/с /(1000)
            case 2: sens=std::abs((1/(1E6))*doseRate/I);break; // (мкГр/ч)/(Кл/с)=Гр/с  /(1000000)
            case 3: sens=std::abs((1/(3.6E3))*doseRate/I);break; // (Гр/ч)/(Кл/с)=Гр/с /(3600)
            case 4: sens=std::abs((1/(3.6E6))*doseRate/I);break; // (мГр/ч)/(Кл/с)=Гр/с /(3600000)
            case 5: sens=std::abs((1/(3.6E9))*doseRate/I);break; // (мкГр/ч)/(Кл/с)=Гр/с /(3600000000)
        default: break;
        }

        QString sensibility = QString::number(sens);
        ui->lineEdit_2->setText(sensibility);
    }
}

void Gamma_IK_verify::on_pushButton_4_clicked() //кнопка записать
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
            if (MesParam==0){qStr="UPDATE gammaChambers SET sensibilityMAED='%1' WHERE name='%2'";}
            else if (MesParam==1){qStr="UPDATE gammaChambers SET sensibilityMNED='%1' WHERE name='%2'";}
            else if (MesParam==2){qStr="UPDATE gammaChambers SET sensibilityMIED='%1' WHERE name='%2'";}
            else if (MesParam==3){qStr="UPDATE gammaChambers SET sensibilityKERMA='%1' WHERE name='%2'";}
            else if (MesParam==4){qStr="UPDATE gammaChambers SET sensibilityMPD='%1' WHERE name='%2'";}
            else if (MesParam==5){qStr="UPDATE gammaChambers SET sensibilityMED='%1' WHERE name='%2'";}

            QString qStr1=qStr.arg(ui->lineEdit_2->text()).arg(chamberName);
            qDebug()<<IKgamma->getBlockName();
            if(!query.exec(qStr1)){qDebug()<<"unable execute query UPDATE gammaChambers";db.lastError().text();}
            db.close();
        }
        QMessageBox::warning(this,"Внимание","Значение чувствительности было изменено",QMessageBox::Ok);
    }
}
