#ifndef SETTINGS_VERIFIER_GAMMA_IK_H
#define SETTINGS_VERIFIER_GAMMA_IK_H

#include "verifygammachambers.h"
#include "password.h"
#include <QDialog>

namespace Ui {
class Settings_verifier_gamma_ik;
}

class Settings_verifier_gamma_ik : public QDialog
{
    Q_OBJECT

public:
    explicit Settings_verifier_gamma_ik(QWidget *parent = nullptr);
    ~Settings_verifier_gamma_ik();

public slots:
    void RefreshSignal();

private slots:
    void on_pushButton_4_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::Settings_verifier_gamma_ik *ui;
    void init();
};

#endif // SETTINGS_VERIFIER_GAMMA_IK_H
