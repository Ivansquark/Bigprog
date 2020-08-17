#ifndef BETA_H
#define BETA_H

#include "beta_measuring.h"
#include "electrometer_test.h"

#include <QDialog>

namespace Ui {
class Beta;
}

class Beta : public QDialog
{
    Q_OBJECT

public:
    explicit Beta(QWidget *parent = nullptr);
    ~Beta();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::Beta *ui;
    Electrometer_test* elTest{nullptr};
    Beta_measuring* betaMes{nullptr};
    void init();
};

#endif // BETA_H
