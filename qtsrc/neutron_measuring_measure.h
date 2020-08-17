#ifndef NEUTRON_MEASURING_MEASURE_H
#define NEUTRON_MEASURING_MEASURE_H

#include "abstract_measurement_measurement.h"
//#include "block_bdg.h"
#include "auto_measurement_result.h"
#include "block_bdkn.h"

#include <QDialog>
#include <QTimer>

namespace Ui {
class Neutron_measuring_measure;
}

class Neutron_measuring_measure : public QDialog, public Abstract_measurement_measurement
{
    Q_OBJECT

public:
    explicit Neutron_measuring_measure(QWidget *parent = nullptr,const QString dimension=nullptr,
                                       const int dimensionForTime=0,
                                       bool regime=0, const uint timesAutomatic=0,
                                       const uint timeAutomatic=0, const uint8_t threashold=0,
                                       const uint timeThreashold=0, const double doseThreashold=0);
    ~Neutron_measuring_measure() override;

public slots:
    void errorConnectionSlot();

private slots:
    void timeOut();
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

public:
    void neutron(Block_BDKN*);

private:
    Ui::Neutron_measuring_measure *ui;
    Block_BDKN* bdkn{nullptr};
    QTimer* timer{nullptr};
    Auto_measurement_result* autoResult{nullptr};

    void init();
    void autoModeResult(double,double,double,double) override; //метод вызываемый в конце автоматического измерения (передаем в него среднюю мощность дозы или ПП, среднюю дозу или Поток, и их СКО)

    double doseRate{0};
    double dose{0};
    ulong time{0};

    QString blockName{""};
    QString Value{""};
    QString IntegralValue{""};
};

#endif // NEUTRON_MEASURING_MEASURE_H
