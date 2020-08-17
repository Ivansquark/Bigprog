#ifndef BETA_VERIFY_H
#define BETA_VERIFY_H

#include "beta_chamber.h"

#include <cmath>
#include <QDialog>
#include <QTimer>
#include <QMessageBox>


namespace Ui {
class Beta_verify;
}

class Beta_verify : public QDialog
{
    Q_OBJECT

public:
    explicit Beta_verify(QWidget *parent = nullptr);
    ~Beta_verify();

    void getBetaChamber(Beta_chamber* beta);

private slots:
    void timeOut();
    void startTimeOut();
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::Beta_verify *ui;
    Beta_chamber* betaChamber{nullptr};
    QTimer* timer{nullptr};
    QTimer* startTimer{nullptr};

    void init();

    long time{0};
    double I{0};
    double Isum{0};
    double avI{0};
    double otkl{0};
    double StandardDeviation{0};
    QString chamberName{""};
};

#endif // BETA_VERIFY_H
