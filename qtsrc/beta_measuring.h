#ifndef BETA_MEASURING_H
#define BETA_MEASURING_H

#include "beta_verify.h"
#include "beta_measuring_measure.h"
#include "password.h"
#include "emulelectrometer.h"
#include "averageadc.h"
#include "beta_chamber.h"
#include "electrometer_test.h"
#include "volthex.h"
#include "chambercompensation.h"

#include <QDialog>
//#include <QRect>
#include <QMessageBox>

namespace Ui {
class Beta_measuring;
}

class Beta_measuring : public QDialog
{
    Q_OBJECT

public:
    explicit Beta_measuring(QWidget *parent = nullptr);
    ~Beta_measuring();

signals:
    void sendToComSig(const QByteArray &);

public slots:
    void testFault();
    void compensationFlagSlot();      //слот выставления флага о том что была проведена компенсация утечек с камерой (при изменении камеры сбросить флаг)

private slots:
    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

    void on_lineEdit_2_textEdited(const QString &arg1);

    void on_comboBox_2_currentIndexChanged(int index);

    void on_radioButton_2_clicked();

    void on_radioButton_3_clicked();

    void on_radioButton_clicked();

    void on_pushButton_4_clicked();

    void on_comboBox_currentIndexChanged(int index);

    void on_checkBox_clicked();

private:
    Ui::Beta_measuring *ui;  
    //AverageADC* AvADC{nullptr};
    Beta_chamber* betaChamber{nullptr};
    Electrometer_test* elTest{nullptr};
    ChamberCompensation* chambComp{nullptr};
    void init();

    int voltage{0};
    bool compensation{false}; //флаг о проведенной компенсации для данной камеры

};

#endif // BETA_MEASURING_H
