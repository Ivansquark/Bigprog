//*********************  Гамма излучение - БДГ - измерение  *************************************

#include "gamma_bdg_measuring_measure.h"
#include "ui_gamma_bdg_measuring_measure.h"

Gamma_BDG_measuring_measure::Gamma_BDG_measuring_measure(QWidget *parent,const QString dimension,
                                                         const int dimensionForTime,
                                                         bool regime, const uint timesAutomatic,
                                                         const uint timeAutomatic, const uint8_t threashold,
                                                         const uint timeThreashold, const double doseThreashold)
    :
    QDialog(parent),
    Abstract_measurement_measurement(dimension, dimensionForTime,regime,
                                      timesAutomatic, timeAutomatic,
                                      threashold, timeThreashold, doseThreashold),
    ui(new Ui::Gamma_BDG_measuring_measure)
{
    ui->setupUi(this);
    init();        
}

Gamma_BDG_measuring_measure::~Gamma_BDG_measuring_measure()
{   
    delete timer;
    delete blockBDG;
    //blockBDG=nullptr;
    delete ui;
}

void Gamma_BDG_measuring_measure::init()
{
    this->setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle("Гамма излучение - БДГ - измерение");    
    ui->label_22->setStyleSheet("font-size:100px; color: red;");
    ui->label_25->setStyleSheet("font-size:100px; color: red;");
    ui->label_21->setStyleSheet("font-size:50px; color: yellow;");
    ui->label_3->setStyleSheet("font-size:50px; color: cyan;");

    ui->label_11->setStyleSheet("font-size:20px; color: yellow;");
    timer = new QTimer(this);
    connect(timer,&QTimer::timeout,this,&Gamma_BDG_measuring_measure::timeOut);

    //switch (DimensionForTime) //проверяем размерность по времени
    //{
    //case 1:timeCoef=60;break;    //для минут
    //case 2:timeCoef=3600;break;  //для часов
    //default:timeCoef=1;break;
    //}
    //int x=Dimension.indexOf("/"); //находим индекс первого элемента /
    //DimensionDose=Dimension.left(x); // размерность дозы (обрезаем до элемента /)

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
}

void Gamma_BDG_measuring_measure::autoModeResult(double AverageDoseRate, double DoseRate_deviation_average,
                                                 double AverageDose, double Dose_deviation_average)
{
    //timer->stop();
    autoInitialization ="Гамма излучение, блок БДГ\n" + blockName +
            "\nКоличество измерений: "+QString::number(TimesAutomatic) +
            "\nВремя измерений: "+QString::number(TimeAutomatic)+" c";
    autoResult = new Auto_measurement_result(this,Value,IntegralValue,autoInitialization);
    autoResult->setModal(true);
    doseTo4(AverageDoseRate,Dimension,autoResult->avDoseRate,autoResult->avDoseRateDimension);//выводим значения в окне объекта другого класса
    doseTo4(AverageDose,DimensionDose,autoResult->avDose,autoResult->avDoseDimension);        //выводим значения в окне объекта другого класса
    autoResult->avDoseRateDeviation->setText(QString::number(DoseRate_deviation_average) +" %");
    autoResult->avDoseDeviation->setText(QString::number(Dose_deviation_average) +" %");
    autoResult->show();
}

void Gamma_BDG_measuring_measure::on_pushButton_clicked() //назад
{
    close();
}

void Gamma_BDG_measuring_measure::timeOut() //основная ф-я измерения (обновляется раз в секунду)
{
//------------------------------------Время-------------------------------------------------------
    time++;
    ui->label_3->setText(QString::number(time) + " c"); //отображение времени
//------------------------------------Мощность дозы-------------------------------------------------------
    doseRate = double(blockBDG->getDoseRate()); //считываем измерение из класса Beta_chamber в чето/с
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

void Gamma_BDG_measuring_measure::BDG(Block_BDG *pointer) //прием указателя на объект блока БДГ (чтобы можно было бы обращаться к объекту из этого класса)
{
    blockBDG=pointer;
    blockName=blockBDG->getBlockName();
    ui->label_5->setText(blockName);
    switch (blockBDG->getMesParam())
    {
        case 0:
        Value="Мощность Амбиентного Эквивалента Дозы";
        IntegralValue = "Амбиентный Эквивалент Дозы";break;
        case 1:
        Value="Мощность Направленного Эквивалента Дозы";
        IntegralValue = "Напрвленный Эквивалент Дозы";break;
        case 2:
        Value="Мощность Индивидуального Эквивалента Дозы";
        IntegralValue = "Индивидуальный Эквивалент Дозы";break;
        case 3:
        Value="Мощность КЕРМЫ";
        IntegralValue = "КЕРМА";break;
        case 4:
        Value="Мощность Поглощенной Дозы";
        IntegralValue = "Поглощенная Доза";break;
        case 5:
        Value="Мощность Экспозиционной Дозы";
        IntegralValue = "Экспозиционная Дозы";break;
    }
    ui->label->setText(Value);
    ui->label_6->setText(IntegralValue);
}

void Gamma_BDG_measuring_measure::on_pushButton_2_clicked() // старт
{
    ui->pushButton_2->setEnabled(false);
    ui->pushButton->setEnabled(false);
    ui->pushButton_2->setVisible(false);
    ui->pushButton->setVisible(false);

    timer->start(1000);
    time=0;
    ui->label_3->setText(QString::number(time)); //обнуляем строку со временем
    dose=0;
    otkl=0;
    doseForAuto=0;//обнуляем интегральные переменные чтобы не накапливалось
    ui->label_21->setText("");//обнуляем строку с порогами
}

void Gamma_BDG_measuring_measure::on_pushButton_3_clicked() // стоп
{
    ui->pushButton_2->setEnabled(true);
    ui->pushButton->setEnabled(true);
    ui->pushButton_2->setVisible(true);
    ui->pushButton->setVisible(true);
    timer->stop();
}

void Gamma_BDG_measuring_measure::errorConnectionSlot() //окончательный прием сигнала об ошибке подключения.
{
    QMessageBox::warning(this, "Ошибка","Ошибка в подключении");
    close();
}


