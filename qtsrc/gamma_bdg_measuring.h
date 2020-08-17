#ifndef GAMMA_BDG_MEASURING_H
#define GAMMA_BDG_MEASURING_H

#include "gamma_bdg_verify.h"
#include "gamma_bdg_measuring_measure.h"
#include "block_bdg.h"

#include <QDialog>

namespace Ui {
class Gamma_BDG_measuring;
}

class Gamma_BDG_measuring : public QDialog
{
    Q_OBJECT

public:
    explicit Gamma_BDG_measuring(QWidget *parent = nullptr);
    ~Gamma_BDG_measuring();

//static void errorConnection();

signals:
    void errorConnectionSig();

public slots:
    void errorConnectionSlot();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

    void on_lineEdit_2_textEdited(const QString &arg1);

    void on_comboBox_2_currentIndexChanged(int index);

    void on_radioButton_2_clicked();

    void on_radioButton_3_clicked();

    void on_radioButton_clicked();

    void on_comboBox_4_currentIndexChanged(int index);

private:
    Ui::Gamma_BDG_measuring *ui;
    Block_BDG* bdg{nullptr};

    void init();

    //static Gamma_BDG_measuring* pThis;
};
#endif // GAMMA_BDG_MEASURING_H
