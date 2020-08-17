#ifndef NEUTRON_MEASURING_H
#define NEUTRON_MEASURING_H

#include "password.h"
#include "neutron_measuring_measure.h"
#include "neutron_verify.h"
#include "block_bdkn.h"

#include <QDialog>

namespace Ui {
class Neutron_measuring;
}

class Neutron_measuring : public QDialog
{
    Q_OBJECT

public:
    explicit Neutron_measuring(QWidget *parent = nullptr);
    ~Neutron_measuring();

signals:
    void errorConnectionSignal();

public slots:
    void errorConnectionSlot();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

    void on_comboBox_2_currentIndexChanged(int index);

    void on_radioButton_2_clicked();

    void on_radioButton_3_clicked();

    void on_radioButton_clicked();

    void on_comboBox_4_currentIndexChanged(int index);

    void on_lineEdit_2_textEdited(const QString &arg1);

private:
    Ui::Neutron_measuring *ui;
    Block_BDKN* bdkn{nullptr};
    void init();
};

#endif // NEUTRON_MEASURING_H
