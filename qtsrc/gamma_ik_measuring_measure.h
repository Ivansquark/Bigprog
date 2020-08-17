#ifndef GAMMA_IK_MEASURING_MEASURE_H
#define GAMMA_IK_MEASURING_MEASURE_H

#include "gammaik_chamber.h"
#include "auto_measurement_result.h"
#include "abstract_measurement_measurement.h"

#include <QDialog>
#include <QMessageBox>

namespace Ui {
class Gamma_IK_measuring_measure;
}

class Gamma_IK_measuring_measure : public QDialog, public Abstract_measurement_measurement
{
    Q_OBJECT

public:
    explicit Gamma_IK_measuring_measure(QWidget *parent = nullptr,const QString dimension="",const int dimensionForTime=0,
                                         bool regime=0, const uint timesAutomatic=0,
                                         const uint timeAutomatic=0, const uint8_t threashold=0,
                                         const uint timeThreashold=0, const double doseThreashold=0);
    ~Gamma_IK_measuring_measure() override;

    void GammaIK(GammaIK_chamber* ikgamma);

signals:
    void errorConnectionSig();

public slots:
    void errorConnectionSlot();

private slots:
    void timeOut();
    void startTimeOut(); //прогрев

    void on_pushButton_9_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

private:
    Ui::Gamma_IK_measuring_measure *ui;
    GammaIK_chamber* IKgamma{nullptr};                     //указатель на объект класса бета камеры из которого берем результат в Гр/с
    Auto_measurement_result* autoResult{nullptr};    //указатель на объект класса результата авто измерения в котором выводим результаты
    QTimer *timer{nullptr};
    QTimer *timerStart{nullptr};

    void init();
    void autoModeResult(double,double,double,double) override; //метод вызываемый в конце автоматического измерения (передаем в него среднюю мощность дозы, среднюю дозу, и их СКО)


    ulong time{0};
    double doseRate{0};
    double dose{0};    // переменная являющаяся интегралом от мощности дозы (выводящаяся на экран) и обнуляющаяся при автоматическом измерении

    QString blockName{""};
    QString Value{""};
    QString IntegralValue{""};
};

#endif // GAMMA_IK_MEASURING_MEASURE_H
