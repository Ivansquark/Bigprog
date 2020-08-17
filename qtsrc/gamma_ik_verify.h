#ifndef GAMMA_IK_VERIFY_H
#define GAMMA_IK_VERIFY_H

#include "gammaik_chamber.h"

#include <cmath>
#include <QDialog>
#include <QMessageBox>

namespace Ui {
class Gamma_IK_verify;
}

class Gamma_IK_verify : public QDialog
{
    Q_OBJECT

public:
    explicit Gamma_IK_verify(QWidget *parent = nullptr);
    ~Gamma_IK_verify();

    void getGammaChamber(GammaIK_chamber* beta);


private slots:
    void timeOut();
    void startTimeOut();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::Gamma_IK_verify *ui;
    GammaIK_chamber* IKgamma{nullptr};
    QTimer* timer{nullptr};
    QTimer* startTimer{nullptr};

    void init();

    long time{0};
    double I{0};
    double Isum{0};
    double avI{0};
    double otkl{0};
    double StandardDeviation{0};

    quint8 MesParam{0};
    QString chamberName{""};
};

#endif // GAMMA_IK_VERIFY_H
