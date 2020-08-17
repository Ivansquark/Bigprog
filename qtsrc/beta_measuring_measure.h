#ifndef BETA_MEASURING_MEASURE_H
#define BETA_MEASURING_MEASURE_H

#include "password.h"
#include "emulelectrometer.h"
#include "beta_chamber.h"
#include "auto_measurement_result.h"
#include "abstract_measurement_measurement.h"

#include <cmath>
#include <QDialog>
#include <QTimer>
#include <QLabel>
#include <QMessageBox>

namespace Ui {
class Beta_measuring_measure;
}

class Beta_measuring_measure : public QDialog, public Abstract_measurement_measurement //множественное наследование
{
    Q_OBJECT

public:
    //конструктор принимающий кучу параметров из предыдущего окна:
    explicit Beta_measuring_measure(QWidget *parent = nullptr,const QString dimension="",const int dimensionForTime=0,
                                    bool regime=0, const uint timesAutomatic=0,
                                    const uint timeAutomatic=0, const uint8_t threashold=0,
                                    const uint timeThreashold=0, const double doseThreashold=0);
    ~Beta_measuring_measure() override;

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void timeOut(); //основная функция измерения в данном классе, обновляющаяся каждую секунду
    void startTimeOut(); //прогрев

public:
    void Beta(Beta_chamber*); //прием указателя на объект бета камеры (чтобы можно было бы обращаться к объекту из этого класса)

private:
    void init();    
    void autoModeResult(double,double,double,double) override; //метод вызываемый в конце автоматического измерения (передаем в него среднюю мощность дозы, среднюю дозу, и их СКО)


    Ui::Beta_measuring_measure *ui;        
    QTimer *timer{nullptr};
    QTimer *timerStart{nullptr};
    QTimer* timerMsg{nullptr};
    Beta_chamber* beta{nullptr};                     //указатель на объект класса бета камеры из которого берем результат в Гр/с
    Auto_measurement_result* autoResult{nullptr};    //указатель на объект класса результата авто измерения в котором выводим результаты
    //QMessageBox* startMsg{nullptr};
    QDialog* dia{nullptr};
    ulong time{0};
    double doseRate{0};
    double dose{0};    // переменная являющаяся интегралом от мощности дозы (выводящаяся на экран) и обнуляющаяся при автоматическом измерении

};

#endif // BETA_MEASURING_MEASURE_H
