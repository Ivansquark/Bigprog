//************************************  Измерение поглощенной дозы бета излучения  *********************

#include "beta_measuring_measure.h"
#include "ui_beta_measuring_measure.h"
#include <QThread>

Beta_measuring_measure::Beta_measuring_measure(QWidget *parent, const QString dimension, const int dimensionForTime,
                                               bool regime, const uint timesAutomatic,
                                               const uint timeAutomatic, const uint8_t threashold,
                                               const uint timeThreashold, const double doseThreashold)
    :
    QDialog(parent),
    Abstract_measurement_measurement(dimension, dimensionForTime,regime,
                                      timesAutomatic, timeAutomatic,
                                      threashold, timeThreashold, doseThreashold),
    ui(new Ui::Beta_measuring_measure)

{
    ui->setupUi(this);
    init();
}

Beta_measuring_measure::~Beta_measuring_measure()
{
    qDebug()<<"~Beta_measuring_measure()";
    delete timer;
    delete beta;
    delete ui;
    //delete startMsg;
    delete timerMsg;
}

void Beta_measuring_measure::init()
{
    this->setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle("Измерение поглощенной дозы бета излучения");
    ui->label_22->setStyleSheet("font-size:100px; color: red;");
    ui->label_25->setStyleSheet("font-size:100px; color: red;");
    ui->label_21->setStyleSheet("font-size:50px; color: yellow;");
    ui->label_3->setStyleSheet("font-size:50px; color: cyan;");

    ui->label_11->setStyleSheet("font-size:20px; color: yellow;");
    value="Мощность поглощенной дозы";
    integralValue="Поглощенная доза";
    ui->label->setText(value);
    ui->label_6->setText(integralValue);

    timer=new QTimer(this);
    connect(timer,&QTimer::timeout,this,&Beta_measuring_measure::timeOut); //коннект на ф-ю timeOut
    timerStart=new QTimer(this);
    connect(timerStart,&QTimer::timeout,this,&Beta_measuring_measure::startTimeOut); //коннект на ф-ю timeOut

    if(Regime==true&&TimeAutomatic!=0&&TimesAutomatic!=0)//если был выбран автоматический режим
    {
        ui->label_11->setText("Автоматический");
        ui->label_12->setText("Номер");
        ui->label_13->setText("0");
        ui->label_14->setText("из");
        ui->label_15->setText(QString::number(TimesAutomatic));
        ui->label_16->setText("время");
        ui->label_17->setText("0");
        ui->label_18->setText("порог времени");
        ui->label_19->setText(QString::number(TimeAutomatic));
    }
    else
    {
        ui->label_11->setText("Ручной");
        ui->label_12->setText("");
        ui->label_13->setText("");
        ui->label_14->setText("");
        ui->label_15->setText("");
        ui->label_16->setText("");
        ui->label_17->setText("");
        ui->label_18->setText("");
        ui->label_19->setText("");
    }
    /*! <Окно с задержкой выполнения программы> */
    QMessageBox* startMsg = new QMessageBox(QMessageBox::Icon::Information,"","",QMessageBox::NoButton,this,Qt::FramelessWindowHint);
    startMsg->setText("Произодится конфигурирование оборудования");
    startMsg->setStandardButtons(0);
    startMsg->setStyleSheet("background-color: rgb(180,178,177);");
    QTimer::singleShot(4000,startMsg,SLOT(hide()));
    startMsg->exec();
    delete startMsg;
}

void Beta_measuring_measure::Beta(Beta_chamber* x) //прием указателя на объект бета камеры (чтобы можно было бы обращаться к объекту из этого класса)
{
    beta=x;
    chamberName=beta->getChamberName();
    //if(QSqlDatabase::contains("myDB"))  //
    //{
    //    QSqlDatabase db=QSqlDatabase::database("myDB");
    //    db.setDatabaseName("config.db");
    //    if(!db.open()){qDebug()<<"db was not opened";}
    //    QSqlQuery query(db);
    //    QString qStr="SELECT *FROM betaChambers WHERE name='%1'";
    //    QString qStr1=qStr.arg(chamberName);
    //    if(!query.exec(qStr1)){qDebug()<<"unable execute query SELECT sensibility FROM betaChambers WHERE name='%1'";}
    //    if (query.first())
    //    {
    //****************************************************************************
    //        //отсюда берем характеристики (имя камеры, напряжение...) которые отобразим в окне измерения
    //****************************************************************************
    //    }
    //    db.close();
    //}    
}

//*////////////////////////////////////////////////////////////////////////////////////////////////
void Beta_measuring_measure::timeOut() //основная функция измерения (раз в секунду)
{
    //qDebug()<<beta->MPD();

//------------------------------------Время-------------------------------------------------------
    time++;
    ui->label_3->setText(QString::number(time) + " c"); //отображение времени
//------------------------------------Мощность дозы-------------------------------------------------------
    doseRate = beta->MPD(); //считываем измерение из класса Beta_chamber в Гр/с
    //qDebug()<<beta->getIres();
    //qDebug()<<beta->MPD();
    double currentDoseRate = doseRate*timeCoef; //для вывода на экран, Мощности Дозы в Гр/с, Гр/мин, Гр/ч
    doseTo4(currentDoseRate,Dimension,ui->label_9,ui->label_2); //передаем в функцию вывода на экран:
    //значение (которое преобразуется),размерность,
    //указатель на label в который записывется преобразованная величина,
    //указатель на label в который записывется преобразованная размерность,

//------------------------------------Доза-------------------------------------------------------
    dose+=doseRate;         // обнуляется при автоматическом измерении, используется для подсчета среднего по дозе
    doseForAuto+=doseRate;  // не обнуляется при автоматическом измерении, используется для рассчета СКО
    doseTo4(dose,DimensionDose,ui->label_8,ui->label_7);

//------------------------------------СКО и Среднее-------------------------------------------------------

    avDoseRate=(doseForAuto/time)*timeCoef;// - средняя величина мощности дозы
    //double avDoseRate = dose*timeCoef;
    doseTo4(avDoseRate,Dimension,ui->label_22,ui->label_25); //выводим на экран среднее
    otkl+=doseRate*doseRate*timeCoef*timeCoef; //сумма квадратов случайной величины
    if(time>0)
    {
        double z=sqrt((otkl/time)-avDoseRate*avDoseRate); //CKO по хитрой формуле: корень из D=(1/N)SUM(1,N,Xi^2)-M^2
        // формула получена на основании того что СКО - это корень из дисперсии
        // Дисперсия - математическое ожидание квадрата отклонения случайной величины от её математического ожидания
        // D=M[X-M(X)]^2 = M(X^2) - M^2(X); - особенное свойство которое везде доказывается
        // M(X^2) - для равновероятных событий является = (1/N)SUM(1,N,doseRate*doseRate)
        // Дисперсия равняется мат ожиданию квадрата случайной величины минус квадрат мат ожидания случайной величины
        // отсюда и получается данная формула: корень из D=(1/N)SUM(1,N,Xi^2)-(среднее)^2

        StandardDeviation=std::abs(z/avDoseRate); //под СКО подразумеваем СКО деленное на среднее и умножить на 100% (относительное отклонение от среднего)
        ui->label_20->setText(QString::number(StandardDeviation*100,'f',3)+" %");
    }

//------------------------------Автоматическое измерение-------------------------------------------------------
    if(Regime==true&&TimeAutomatic!=0&&TimesAutomatic!=0)   //условие автоматического измерения (при автоматическом измерении пороги не срабатывают)
    {
        autoMode(dose,timer,ui->label_17,ui->label_13);
    }
    else // (если нет авто измерения проверяем пороги)
    {
    //------------------------------------Пороги-------------------------------------------------------
        timeThreashold(time,ui->label_21,timer);   //проверка на порог по времени
        doseThreashold(dose,ui->label_21,timer);   //проверка на порог по дозе
    }
}
//*////////////////////////////////////////////////////////////////////////////////////////////////


void Beta_measuring_measure::autoModeResult(double AverageDoseRate, double DoseRate_deviation_average,
                                            double AverageDose, double Dose_deviation_average)
{
    //timer->stop();
    autoInitialization ="Бета излучение\n" + chamberName +
            "\nКоличество измерений: "+QString::number(TimesAutomatic) +
            "\nВремя измерений: "+QString::number(TimeAutomatic)+" c";
    autoResult = new Auto_measurement_result(this,value,integralValue, autoInitialization);
    autoResult->setModal(true);
    doseTo4(AverageDoseRate,Dimension,autoResult->avDoseRate,autoResult->avDoseRateDimension);//выводим значения в окне объекта другого класса
    doseTo4(AverageDose,DimensionDose,autoResult->avDose,autoResult->avDoseDimension);        //выводим значения в окне объекта другого класса
    autoResult->avDoseRateDeviation->setText(QString::number(DoseRate_deviation_average) +" %");
    autoResult->avDoseDeviation->setText(QString::number(Dose_deviation_average) +" %");
    autoResult->show();
}

void Beta_measuring_measure::on_pushButton_clicked() //назад
{
    close();
    //delete this; //вызываем деструктор (если эмулятор)
    //this->~Beta_measuring_measure();//(тоже что и предыдущая строка - закрытие измерения с удалением всех данных эмулятора)
}

void Beta_measuring_measure::on_pushButton_2_clicked() //старт
{
    ui->pushButton_2->setEnabled(false);
    ui->pushButton->setEnabled(false);
    ui->pushButton_3->setEnabled(false);
    ui->pushButton_2->setVisible(false);
    ui->pushButton->setVisible(false);

    timerStart->setSingleShot(true);
    timerStart->start(60000);
    ui->label_3->setText(QString::number(time));
    ui->label_21->setText("ПРОГРЕВ!!!");
}

void Beta_measuring_measure::startTimeOut() //прогрев
{
    ui->pushButton_3->setEnabled(true);
    timer->start(1000);
    time=0;
    ui->label_3->setText(QString::number(time));
    dose=0;
    otkl=0;
    doseForAuto=0;//обнуляем интегральные переменные чтобы не накапливалось
    ui->label_21->setText("");
}


void Beta_measuring_measure::on_pushButton_3_clicked() //стоп
{
    ui->pushButton_2->setEnabled(true);
    ui->pushButton->setEnabled(true);
    ui->pushButton_2->setVisible(true);
    ui->pushButton->setVisible(true);
    timer->stop();
}
