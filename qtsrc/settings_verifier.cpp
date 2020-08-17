//**************************************** настройки поверителя  *********************
#include "settings_verifier.h"
#include "ui_settings_verifier.h"

Settings_verifier::Settings_verifier(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings_verifier)
{
    ui->setupUi(this);
    setWindowTitle("Настройки поверителя");
}

Settings_verifier::~Settings_verifier()
{
    delete ui;
}

void Settings_verifier::on_pushButton_5_clicked() //btn назад
{
    close();
}

void Settings_verifier::on_pushButton_clicked() // Бета излучение
{
    Settings_verifier_beta* setVerBeta = new Settings_verifier_beta(this);
    setVerBeta->setModal(true);
    setVerBeta->show();

}

void Settings_verifier::on_pushButton_2_clicked() // Гамма излучение (ИК)
{
    Settings_verifier_gamma_ik *setVerGammaIK = new Settings_verifier_gamma_ik(this);
    setVerGammaIK->setModal(true);
    setVerGammaIK->show();
}

void Settings_verifier::on_pushButton_3_clicked()  // Гамма излучение (БДГ)
{    
    Settings_verifier_gamma_nuclides * setVerGammaNucl = new Settings_verifier_gamma_nuclides(this);
    setVerGammaNucl->setModal(true);
    setVerGammaNucl->show();
}

void Settings_verifier::on_pushButton_4_clicked()
{
    Settings_verifier_neutron *setVerNeutron = new Settings_verifier_neutron(this);
    setVerNeutron->setModal(true);
    setVerNeutron->show();
}
