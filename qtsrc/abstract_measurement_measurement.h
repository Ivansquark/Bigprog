//**************************** настоящий Абстрактный класс измерения (в нем осуществляется вывод на экран, проверяются пороги и реализуется автоматическое измерение)***************
//******
//*************************** класс абстрактный - т.к. использует чисто виртуальную функцию => у него нельзя создать объекты и класс предоставляет только интерфейс **************

#ifndef ABSTRACT_MEASUREMENT_MEASUREMENT_H
#define ABSTRACT_MEASUREMENT_MEASUREMENT_H

#include <QLabel>
#include <cmath>
#include <QTimer>
#include <QDebug>

class Abstract_measurement_measurement //класс участвует в множественном наследовании, соответственно он не должен быть наследником QObject иначе возможны конфликты
{
public:
    Abstract_measurement_measurement(const QString dimension="",const int dimensionForTime=0,
                                     bool regime=0, const uint timesAutomatic=0,
                                     const uint timeAutomatic=0, const uint8_t threashold=0,
                                     const uint timeThreashold=0, const double doseThreashold=0);
   virtual ~Abstract_measurement_measurement();

protected:
    void init();
    void timeThreashold(ulong &time,QLabel* threashold,QTimer* timer); //функция останавливающая измерение при достижении порога по времени
    void doseThreashold(double &dose,QLabel* threashold,QTimer* timer,bool Fluence=false); //функция останавливающая измерение при достижении порога по дозе (имеется параметр по умолчанию для флюенса)
    void doseShow(double&, QLabel*) const;//метод отображающий ПД на экране (вспомогательный, вызывается из doseTo4)

// метод включающий в себя два параметра по умолчанию необходимые для передачи флагов измерения Плотности потока и Флюенса:
    void doseTo4(double&,const QString&, QLabel*, QLabel*, bool PP=0,bool Fluence=0) const; //метод переводящий Гр в м, мк, н... для удобства чтения (вызывает в себе метод doseShow() )

    void autoMode(double &dose, QTimer *timer,QLabel* time,QLabel* times);       //метод вызываемый если вызвано автоматическое измерение

//------- Далее описан чисто виртуальный метод который необходимо обязательно переопределить в классах потомках (для каждого класса потомка он будет свой и содержать свои параметры)
    /////////////////////////////////////////////////////////////////////////////////////
    virtual void autoModeResult(double,double,double,double)=0; //метод вызываемый в конце автоматического измерения (передаем в него среднюю мощность дозы (ПП), среднюю дозу (Флюенс), и их СКО)
    /////////////////////////////////////////////////////////////////////////////////////

    //--------  Переменные принимаемые из предыдущего окна {например beta_measuring} (инициализируются реальными значениями в конструкторе) -------------
        const QString Dimension{""}; //размерность измеряемой величины (мощности дозы)
        QString DimensionDose{""};   //размерность дозы (для вывода на экран)
        const int DimensionForTime{0}; //размерность от времени (0-Гр/с), (1-Гр/мин), (2-Гр/ч)
        bool Regime{false};  //режим - ручной(0), автоматический (1)
        const uint TimesAutomatic{0}; //количество измерений при автоматическом режиме
        const uint TimeAutomatic{0}; //время автоматического измерения
        const uint8_t Threashold{0}; // режим порога нет(0), по времени(1), по дозе(2)
        const uint TimeThreashold{0}; // количество секунд в пороге по времени
        const double DoseThreashold{0}; // доза при пороге по дозе
    //----------------------------------------------------------------------------------------------------------
        QString chamberName{""};
        QString value{""};
        QString integralValue{""};

        double timeCoef{0}; //коэффициент зависящий от размерности по времени ()

    //*****СКО************
        double avDoseRate{0}; // средняя мощность дозы, от которой рассчитывается СКО
        double otkl{0};
        double StandardDeviation{0};
    //****** Автоматическое измерение ********
        QString autoInitialization{""};  //переменная в которую записываем информацию
        uint automode_one_time_current{0}; // время измерения в автоматическом режиме (выводится вверху справа)
        uint automode_times_current{0};    // номер измерения в автоматическом режиме (выводится вверху справа)
        double automode_DoseRate_current{0}; //переменная в которой складываются средние значения мощности дозы (нужна для расчета среднего)
        double automode_Dose_current{0};   //переменная в которой складываются средние значения дозы (нужна для расчета среднего)
        double automode_DoseRate_deviation{0}; //переменная матожидание от квадрата (для рассчетаСКО в авто режиме)
        double automode_Dose_deviation{0};     // тоже самое только для дозы
        double doseForAuto{0};    //дополнительные переменные которые участвуют в рассчете СКО и не обнуляются при автоматическом измерении

private:
        QTimer * timer;

};

#endif // ABSTRACT_MEASUREMENT_MEASUREMENT_H
