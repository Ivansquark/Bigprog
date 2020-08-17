#ifndef AUTO_MEASUREMENT_RESULT_H
#define AUTO_MEASUREMENT_RESULT_H

#include <QDialog>
#include <QLabel>

namespace Ui {
class Auto_measurement_result;
}

class Auto_measurement_result : public QDialog
{
    Q_OBJECT

public:
    explicit Auto_measurement_result(QWidget *parent = nullptr, QString value={""}, QString integralValue={""},
                                     QString initialization = nullptr);
    ~Auto_measurement_result();

    QLabel* avDoseRate{nullptr}; //создаем указатели на label-ы, чтобы потом передать их в функцию вывода числа в виде (0.000) и размерности
    QLabel* avDoseRateDimension{nullptr};
    QLabel* avDose{nullptr};
    QLabel* avDoseDimension{nullptr};
    QLabel* avDoseRateDeviation{nullptr};
    QLabel* avDoseDeviation{nullptr};


private slots:
    void on_pushButton_clicked();

private:
    Ui::Auto_measurement_result *ui;
    void init();

    QString Value{""}; //наименование измеряемой величины (для бета - мощность поглощенной дозы)
    QString IntegralValue{""}; //наименование интеграла по времени измеряемой величины (для бета - поглощенная доза)

    QString Initialization;
};

#endif // AUTO_MEASUREMENT_RESULT_H
