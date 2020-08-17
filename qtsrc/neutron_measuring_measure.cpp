//************************  "Нейтронное излучение - измерение"  **********************************

#include "neutron_measuring_measure.h"
#include "ui_neutron_measuring_measure.h"

Neutron_measuring_measure::Neutron_measuring_measure(QWidget *parent,
                                                     const QString dimension,
                                                     const int dimensionForTime,
                                                     bool regime, const uint timesAutomatic,
                                                     const uint timeAutomatic, const uint8_t threashold,
                                                     const uint timeThreashold, const double doseThreashold) :
    QDialog(parent),
    Abstract_measurement_measurement(dimension, dimensionForTime,regime,
                                      timesAutomatic, timeAutomatic,
                                      threashold, timeThreashold, doseThreashold),
    ui(new Ui::Neutron_measuring_measure)
{
    ui->setupUi(this);
    init();
}

Neutron_measuring_measure::~Neutron_measuring_measure()
{
    qDebug()<<"delete bdkn;";
    delete bdkn;
    qDebug()<<"delete timer;";
    delete timer;
    delete ui;
    qDebug()<<"~Neutron_measuring_measure()";
}

void Neutron_measuring_measure::init()
{
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle("Нейтронное излучение - измерение");
    ui->label_22->setStyleSheet("font-size:100px; color: red;");
    ui->label_25->setStyleSheet("font-size:25px; color: red;");
    //ui->label_21->setStyleSheet("font-size:50px; color: yellow;");
    ui->label_3->setStyleSheet("font-size:50px; color: cyan;");

    ui->label_11->setStyleSheet("font-size:20px; color: yellow;");
    timer = new QTimer(this);
    connect(timer,&QTimer::timeout,this,&Neutron_measuring_measure::timeOut);

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

void Neutron_measuring_measure::timeOut()
{
    //------------------------------------Время-------------------------------------------------------
        time++;
        ui->label_3->setText(QString::number(time) + " c"); //отображение времени
    //------------------------------------Мощность дозы-------------------------------------------------------
        doseRate = double(bdkn->getDoseRate()); //считываем измерение из класса Beta_chamber в чето/с
        //qDebug()<<beta->MPD();
        double currentDoseRate = doseRate*timeCoef; //для вывода на экран, Мощности Дозы в Гр/с, Гр/мин, Гр/ч
        if (Value=="Плотность Потока")
        {
            doseTo4(currentDoseRate,Dimension,ui->label_9,ui->label_2,true,false); //передаем в функцию вывода на экран:
        }
        else doseTo4(currentDoseRate,Dimension,ui->label_9,ui->label_2,false,false);

        //значение (которое преобразуется),размерность,
        //указатель на label в который записывется преобразованная величина,
        //указатель на label в который записывется преобразованная размерность,

    //------------------------------------Доза-------------------------------------------------------
        dose+=doseRate;         // обнуляется при автоматическом измерении, используется для подсчета среднего по дозе
        doseForAuto+=doseRate;  // не обнуляется при автоматическом измерении, используется для рассчета СКО

        if (Value=="Плотность Потока")
        {
            doseTo4(dose,DimensionDose,ui->label_8,ui->label_7,true,true); //передаем в функцию вывода на экран:
        }
        else doseTo4(dose,DimensionDose,ui->label_8,ui->label_7,false,false);

        //------------------------------------СКО и Среднее-------------------------------------------------------
        avDoseRate=(doseForAuto/time)*timeCoef;// - средняя величина мощности дозы
        //double avDoseRate = dose*timeCoef;
        if (Value=="Плотность Потока")
        {
            doseTo4(avDoseRate,Dimension,ui->label_22,ui->label_25,true,false); //передаем в функцию вывода на экран:
        }
        else doseTo4(avDoseRate,Dimension,ui->label_22,ui->label_25,false,false); //выводим на экран среднее

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
            timeThreashold(time,ui->label_5,timer);   //проверка на порог по времени
            if (Value=="Плотность Потока") // выводим среднюю мощность дозы (или ПП)
            {
                doseThreashold(dose,ui->label_5,timer,true);   //проверка на порог по флюенсу
            }
            else doseThreashold(dose,ui->label_5,timer,false);   //проверка на порог по дозе
        }
}

// --------------------------  Переопределенная функция абстрактного класса ----------------------------------
void Neutron_measuring_measure::autoModeResult(double AverageDoseRate, double DoseRate_deviation_average,
                                               double AverageDose, double Dose_deviation_average)
{
    autoInitialization ="Нейтронное излучение, блок БДКН\n" + blockName +
            "\nКоличество измерений: "+QString::number(TimesAutomatic) +
            "\nВремя измерений: "+QString::number(TimeAutomatic)+" c";
    autoResult = new Auto_measurement_result(this,Value,IntegralValue,autoInitialization);
    autoResult->setModal(true);

    if (Value=="Плотность Потока") // выводим среднюю мощность дозы (или ПП)
    {
        doseTo4(AverageDoseRate,Dimension,autoResult->avDoseRate,autoResult->avDoseRateDimension,true,false); //передаем в функцию вывода на экран:
    }
    else doseTo4(AverageDoseRate,Dimension,autoResult->avDoseRate,autoResult->avDoseRateDimension,false,false); //выводим на экран среднее

    //doseTo4(AverageDoseRate,Dimension,autoResult->avDoseRate,autoResult->avDoseRateDimension,true);//выводим значения в окне объекта другого класса

    if (Value=="Плотность Потока")  // выводим среднюю дозу (или флюенс)
    {
        doseTo4(AverageDose,DimensionDose,autoResult->avDose,autoResult->avDoseDimension,true,true); //передаем в функцию вывода на экран:
    }
    else doseTo4(AverageDose,DimensionDose,autoResult->avDose,autoResult->avDoseDimension,false,false); //выводим на экран среднее

    //doseTo4(AverageDose,DimensionDose,autoResult->avDose,autoResult->avDoseDimension,true);        //выводим значения в окне объекта другого класса
    autoResult->avDoseRateDeviation->setText(QString::number(DoseRate_deviation_average) +" %");
    autoResult->avDoseDeviation->setText(QString::number(Dose_deviation_average) +" %");
    autoResult->show();
}

void Neutron_measuring_measure::on_pushButton_clicked() //назад
{
    timer->stop();
    close();
}

void Neutron_measuring_measure::on_pushButton_2_clicked() //старт
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
    //ui->label_21->setText("");//обнуляем строку с порогами
}

void Neutron_measuring_measure::on_pushButton_3_clicked() //стоп
{
    ui->pushButton_2->setEnabled(true);
    ui->pushButton->setEnabled(true);
    ui->pushButton_2->setVisible(true);
    ui->pushButton->setVisible(true);
    timer->stop();
}

void Neutron_measuring_measure::neutron(Block_BDKN *x)
{
    bdkn=x;
    blockName=bdkn->getBlockName();
    ui->label_5->setText(blockName);
    switch (bdkn->getMesParam())
    {
        case 0:
        Value="Плотность Потока";
        IntegralValue = "Флюенс";break;
        case 1:
        Value="Мощность Поглощенной Дозы";
        IntegralValue = "Поглощенная Доза";break;
        case 2:
        Value="Мощность Амбиентного Эквивалента Дозы";
        IntegralValue = "Амбиентный Эквивалент Дозы";break;
    default:break;
    }
    ui->label->setText(Value);
    ui->label_6->setText(IntegralValue);
}

void Neutron_measuring_measure::errorConnectionSlot()
{
    QMessageBox::warning(this, "Ошибка","Ошибка в подключении");
    close();
}
