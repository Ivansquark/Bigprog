#ifndef GAMMA_IK_MEASURING_H
#define GAMMA_IK_MEASURING_H

#include "gamma_ik_measuring_measure.h"
#include "gamma_ik_verify.h"
#include "password.h"
#include "gammaik_chamber.h"
#include "electrometer_test.h"
#include "chambercompensation.h"
#include "volthex.h"

#include <QDialog>
#include <QDebug>

namespace Ui {
class Gamma_IK_measuring;
}

class Gamma_IK_measuring : public QDialog
{
    Q_OBJECT

public:
    explicit Gamma_IK_measuring(QWidget *parent = nullptr);
    ~Gamma_IK_measuring();

signals:
    void sendToComSig(const QByteArray &);

public slots:
    void testFault();
    void compensationFlagSlot();      //слот выставления флага о том что была проведена компенсация утечек с камерой


private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_comboBox_4_currentIndexChanged(int index);

    void on_lineEdit_2_textEdited(const QString &arg1);

    void on_comboBox_2_currentIndexChanged(int index);

    void on_radioButton_2_clicked();

    void on_radioButton_3_clicked();

    void on_radioButton_clicked();

    void on_pushButton_4_clicked();

    void on_comboBox_currentIndexChanged(int index);

    void on_checkBox_clicked();

private:
    Ui::Gamma_IK_measuring *ui;
    GammaIK_chamber* IKgamma;
    Electrometer_test* elTest{nullptr};
    ChamberCompensation* chambComp{nullptr};

    //AverageADC* AvADC{nullptr};
    void init();

    int voltage{0};
    bool compensation{0};  //флаг о том была ли проведена компенсация
    uint DAC1{0};          // значения ЦАП для компенсации нуля усилителя (из БД в которую записалось при тесте)
};

#endif // GAMMA_IK_MEASURING_H
