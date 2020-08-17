#ifndef SETTINGS_VERIFIER_H
#define SETTINGS_VERIFIER_H

#include "settings_verifier_beta.h"
#include "settings_verifier_gamma_ik.h"
//#include "settings_verifier_gamma_bdg.h"
#include "settings_verifier_gamma_nuclides.h"
#include "settings_verifier_neutron.h"


#include <QDialog>



namespace Ui {
class Settings_verifier;
}

class Settings_verifier : public QDialog
{
    Q_OBJECT

public:
    explicit Settings_verifier(QWidget *parent = nullptr);
    ~Settings_verifier();

private slots:
    void on_pushButton_5_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::Settings_verifier *ui;
};

#endif // SETTINGS_VERIFIER_H
