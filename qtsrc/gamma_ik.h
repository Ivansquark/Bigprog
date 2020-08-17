#ifndef GAMMA_IK_H
#define GAMMA_IK_H

#include "gamma_ik_measuring.h"

#include <QDialog>

namespace Ui {
class Gamma_IK;
}

class Gamma_IK : public QDialog
{
    Q_OBJECT

public:
    explicit Gamma_IK(QWidget *parent = nullptr);
    ~Gamma_IK();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::Gamma_IK *ui;
    void init();
};

#endif // GAMMA_IK_H
