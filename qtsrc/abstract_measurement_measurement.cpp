//**************************** настоящий Абстрактный класс измерения (в нем осуществляется вывод на экран, проверяются пороги и реализуется автоматическое измерение)***************
//****************************
//**************************** класс создан потому что код находящийся в нем используется в 4-х окнах измерения (чтобы постоянно не писать одно и тоже), соответственно они наследуются от него


#include "abstract_measurement_measurement.h"

Abstract_measurement_measurement::Abstract_measurement_measurement(const QString dimension,const int dimensionForTime,
                                                                   bool regime, const uint timesAutomatic,
                                                                   const uint timeAutomatic, const uint8_t threashold,
                                                                   const uint timeThreashold, const double doseThreashold)
    :
    Dimension(dimension), DimensionForTime(dimensionForTime),Regime(regime),
    TimesAutomatic(timesAutomatic), TimeAutomatic(timeAutomatic),
    Threashold(threashold),TimeThreashold(timeThreashold), DoseThreashold(doseThreashold)
{
    init();
}

Abstract_measurement_measurement::~Abstract_measurement_measurement()
{qDebug()<<"~Abstract_measurement_measurement()";}
void Abstract_measurement_measurement::init()
{
    switch (DimensionForTime) //проверяем размерность по времени
    {
    case 1:timeCoef=60;break;    //для минут
    case 2:timeCoef=3600;break;  //для часов
    default:timeCoef=1;break;
    }

    int x=Dimension.indexOf("/"); //находим индекс первого элемента /
    DimensionDose=Dimension.left(x); // размерность дозы (обрезаем до элемента /)
}


void Abstract_measurement_measurement::timeThreashold(ulong &time,QLabel* threashold,QTimer* timer)
{
    if(Threashold==1&&time==TimeThreashold)
    {
        timer->stop();
        threashold->setText("Достигнут порог по времени");
    }
}

void Abstract_measurement_measurement::doseThreashold(double &dose,QLabel* threashold,QTimer* timer,bool Fluence)
{
    if(Fluence==true) //если в нейтронном измерении стоит плотность потока (флюенс не делим на 1000)
    {
        if(Threashold==2&&dose>=DoseThreashold) //част в част
        {
            timer->stop();
            threashold->setText("Достигнут порог по флюенсу");
        }
    }
    else
    {
        if(Threashold==2&&dose>=DoseThreashold/1000) //перевод Гр в мГр
        {
            timer->stop();
            threashold->setText("Достигнут порог по дозе");
        }
    }
}

void Abstract_measurement_measurement::doseShow(double &z, QLabel *fig4) const
{
    if (QString::number(z,'f',3).size()<6)
    {
        fig4->setText(QString::number(z,'f',3));  // отображение 1,111
    }
    else if (QString::number(z,'f',3).size()>=6 && QString::number(z,'f',3).size()<7)
    {
        fig4->setText(QString::number(z,'f',2));  // отображение 11,11
    }
    else if (QString::number(z,'f',3).size()>=7 && QString::number(z,'f',3).size()<8)
    {
        fig4->setText(QString::number(z,'f',1));  // отображение 111,1
    }
    else if (QString::number(z,'f',3).size()>=8)
    {
        fig4->setText(QString::number(z,'f',0));  // отображение 1111
    }
}

void Abstract_measurement_measurement::doseTo4(double &z1,
                                               const QString &dimension,
                                               QLabel *fig4,
                                               QLabel *dim,
                                               bool PP,
                                               bool Fluence) const
{
    if(PP==false) //измеряем не Плотность Потока
    {
        double z = z1*1e12; //  умножаем на 10 в 12
        if (z<1e3)
        {
            dim->setText("п"+dimension);  // отображение (пГр/с) или (пГр)
            doseShow(z,fig4); // функция отображения значения величины
        }
        if (z>=1e3 && z<1e6)
        {
            dim->setText("н"+dimension);  // отображение
            z=z/1e3;
            doseShow(z,fig4); // функция отображения значения величины
        }
        if (z>=1e6 && z<1e9)
        {
            dim->setText("мк"+dimension);  // отображение
            z=z/1e6;
            doseShow(z,fig4); // функция отображения значения величины
        }
        if (z>=1e9 && z<1e12)
        {
            dim->setText("м"+dimension);  // отображение
            z=z/1e9;
            doseShow(z,fig4); // функция отображения значения величины
        }
        if (z>=1e12)
        {
            dim->setText(dimension);  // отображение
            z=z/1e12;
            doseShow(z,fig4);
        }
    }
    else // измеряем Плотность Потока
    {
        if(Fluence==false) //измеряем плотность потока
        {
            dim->setText(dimension);
            fig4->setText(QString::number(z1,'f',2));
        }
        else    //измеряем флюенс (интеграл по времени от плотности потока)
        {
            dim->setText(dimension+"/кв.см");
            fig4->setText(QString::number(z1,'f',0));
        }
    }
}

void Abstract_measurement_measurement::autoMode(double &dose, QTimer *timer,QLabel* time,QLabel* times)
{
    automode_one_time_current+=1; //отсчет времени одного из измерений
    //avDoseRate=(dose/automode_one_time_current)*timeCoef; // - средняя величина
    time->setText(QString::number(automode_one_time_current));
    if(automode_one_time_current==TimeAutomatic) //если доходит до порога по времени
    {
        automode_one_time_current=0; // сбрасываем счетчик времени
        automode_times_current+=1;   // увеличиваем счетчик текущего измерения
        times->setText(QString::number(automode_times_current));

        automode_DoseRate_current+=avDoseRate; // складываем  значения мощностей дозы (средние) для вычисления среднего от всех измерений
        automode_Dose_current+=dose; // складываем значения дозы для вычисления среднего

        automode_DoseRate_deviation+=avDoseRate*avDoseRate; //матожидание от квадрата (для рассчетаСКО мощности дозы)
        automode_Dose_deviation+=dose*dose;                 //матожидание от квадрата (для рассчетаСКО дозы)

        dose=0; //обнуляем дозу (для того чтобы заново набрать на следующем измерении)

        if(automode_times_current==TimesAutomatic) //закончено последнее измерение
        {
            timer->stop(); //останавливаем таймер (т.к. таймер для каждого детектора свой необходимо передавать указатель на него)

            double AverageDoseRate=automode_DoseRate_current/TimesAutomatic;//Среднее ПО ПРОХОДАМ для мощности дозы
            double z=sqrt((automode_DoseRate_deviation/(TimesAutomatic))-AverageDoseRate*AverageDoseRate); //CKO по хитрой формуле D=(1/N)SUM(1,N,ai**2)-M**2
            double DoseRate_deviation_average=std::abs(100*z/AverageDoseRate);//CKO %  мощности дозы

            double AverageDose=automode_Dose_current/TimesAutomatic;//Среднее ПО ПРОХОДАМ для дозы
            double z1=sqrt((automode_Dose_deviation/(TimesAutomatic))-AverageDose*AverageDose); //CKO по хитрой формуле D=(1/N)SUM(1,N,ai**2)-M**2
            double Dose_deviation_average=std::abs(100*z1/AverageDose);//CKO %  дозы

            automode_times_current=0; // все обнуляем, чтобы можно было потом повторить измерение
            doseForAuto=0;
            automode_DoseRate_current=0;
            automode_Dose_current=0;
            automode_DoseRate_deviation=0;
            automode_Dose_deviation=0;
            //вызываем функцию (которая будет определена только в классе потомке) в которую закидываем посчитанные значения
            autoModeResult(AverageDoseRate,DoseRate_deviation_average,AverageDose,Dose_deviation_average); //вызываем чисто виртуальную функцию (для каждого детектора они своя - это пример динамического полиморфизма - <TRUE_OOP>)
        }
    }
}
