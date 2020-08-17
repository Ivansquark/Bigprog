#ifndef SETTINGS_VERIFIER_NEUTRON_H
#define SETTINGS_VERIFIER_NEUTRON_H

#include "password.h"
#include "verifyneutron.h"

#include <QDialog>



namespace Ui {
class Settings_verifier_neutron;
}

class Settings_verifier_neutron : public QDialog
{
    Q_OBJECT

public:
    explicit Settings_verifier_neutron(QWidget *parent = nullptr);
    ~Settings_verifier_neutron();


private slots:
    void addRefresh();    

    void on_pushButton_4_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::Settings_verifier_neutron *ui;
    void init();
};

#endif // SETTINGS_VERIFIER_NEUTRON_H
