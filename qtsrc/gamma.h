#ifndef GAMMA_H
#define GAMMA_H

#include "gamma_bdg.h"
#include "gamma_ik.h"

#include <QDialog>

namespace Ui {
class Gamma;
}

class Gamma : public QDialog
{
    Q_OBJECT

public:
    explicit Gamma(QWidget *parent = nullptr);
    ~Gamma();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::Gamma *ui;
    void init();
};

#endif // GAMMA_H
