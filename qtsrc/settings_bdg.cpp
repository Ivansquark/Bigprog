#include "settings_bdg.h"
#include "ui_settings_bdg.h"

Settings_BDG::Settings_BDG(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings_BDG)
{
    ui->setupUi(this);
}

Settings_BDG::~Settings_BDG()
{
    delete ui;
}

void Settings_BDG::on_pushButton_2_clicked()//назад
{
    close();
}
