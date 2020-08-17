#ifndef NEUTRON_VERIFY_H
#define NEUTRON_VERIFY_H

#include "block_bdkn.h"

#include <QDialog>

namespace Ui {
class Neutron_verify;
}

class Neutron_verify : public QDialog
{
    Q_OBJECT

public:
    explicit Neutron_verify(QWidget *parent = nullptr);
    ~Neutron_verify();

    void getBlockBDKN(Block_BDKN* b);

public slots:
    void errorConnectionSlot();

private slots:
    void timeOut();
    void startTimeOut();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::Neutron_verify *ui;
    Block_BDKN* bdkn{nullptr};
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
    quint8 MesDiap{0};
    QString chamberName{""};
};

#endif // NEUTRON_VERIFY_H
