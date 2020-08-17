//*********************************** Автоматическое измерение ********************************

#include "auto_measurement_result.h"
#include "ui_auto_measurement_result.h"

Auto_measurement_result::Auto_measurement_result(QWidget *parent,QString value, QString integralValue,
                                                 QString initialization) :
    QDialog(parent),
    ui(new Ui::Auto_measurement_result), Value(value), IntegralValue(integralValue), Initialization(initialization)
{
    ui->setupUi(this);
    init();
}

Auto_measurement_result::~Auto_measurement_result()
{
    delete ui;
}

void Auto_measurement_result::on_pushButton_clicked() //назад
{
    close();
}

void Auto_measurement_result::init()
{
    setWindowTitle("Автоматическое измерение");
    ui->label->setText(Initialization);
    avDoseRate=ui->label_3;
    avDoseRateDimension=ui->label_4;
    avDose=ui->label_7;
    avDoseDimension=ui->label_9;
    avDoseRateDeviation=ui->label_6;
    avDoseDeviation=ui->label_11;

    ui->label_2->setText(Value);         //выводим наименование измеряемой величины (для бета - мощность поглощенной дозы)
    ui->label_8->setText(IntegralValue); //выводим наименование интеграла по времени измеряемой величины (для бета - поглощенная доза)
}
