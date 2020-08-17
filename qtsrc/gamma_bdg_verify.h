#ifndef GAMMA_BDG_VERIFY_H
#define GAMMA_BDG_VERIFY_H

#include "block_bdg.h"

#include <QDialog>

namespace Ui {
class Gamma_BDG_verify;
}

class Gamma_BDG_verify : public QDialog
{
    Q_OBJECT

public:
    explicit Gamma_BDG_verify(QWidget *parent = nullptr);
    ~Gamma_BDG_verify();

    void getGammaBlock(Block_BDG*);

public:
    void errorConnectionSlot();

private slots:
    void timeOut();
    void startTimeOut();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::Gamma_BDG_verify *ui;
    Block_BDG* bdg{nullptr};
    QTimer* timer{nullptr};
    QTimer* startTimer{nullptr};
    QMessageBox* msg{nullptr};

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

#endif // GAMMA_BDG_VERIFY_H
