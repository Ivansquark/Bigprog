//*************************** Окно гамма измерения *********************************


#include "gamma.h"
#include "ui_gamma.h"

Gamma::Gamma(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Gamma)
{
    ui->setupUi(this);
    init();
}

Gamma::~Gamma()
{
    delete ui;
}

void Gamma::init()
{
    setWindowTitle("Гамма излучение");
}

void Gamma::on_pushButton_clicked() //назад
{
    close();
}


void Gamma::on_pushButton_2_clicked() //БДГ
{
    Gamma_BDG * gammaBDG = new Gamma_BDG(this);
    gammaBDG->setModal(true);
    gammaBDG->show();

}

void Gamma::on_pushButton_3_clicked() //ИК
{
    Gamma_IK* gammaIK = new Gamma_IK(this);
    gammaIK->setModal(true);
    gammaIK->show();
}
