#include "settings_bdkn.h"
#include "ui_settings_bdkn.h"

Settings_BDKN::Settings_BDKN(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings_BDKN)
{
    ui->setupUi(this);
}

Settings_BDKN::~Settings_BDKN()
{
    delete ui;
}

void Settings_BDKN::on_pushButton_2_clicked()//назад
{
    close();
}
