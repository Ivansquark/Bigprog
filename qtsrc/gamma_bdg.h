#ifndef GAMMA_BDG_H
#define GAMMA_BDG_H

#include "gamma_bdg_test.h"
#include "gamma_bdg_measuring.h"
#include <QDialog>

namespace Ui {
class Gamma_BDG;
}

class Gamma_BDG : public QDialog
{
    Q_OBJECT

public:
    explicit Gamma_BDG(QWidget *parent = nullptr);
    ~Gamma_BDG();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::Gamma_BDG *ui;
    void init();
};

#endif // GAMMA_BDG_H
