//**************************************** настройки изготовителя *********************
#include "settings.h"
#include "ui_settings.h"

Settings::Settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);
    init();
}

Settings::~Settings()
{
    delete ui;    
}

void Settings::on_pushButton_5_clicked()//кнопка назад
{
    close();
}

void Settings::init()
{
    setWindowTitle("Настройки изготовителя");
}

void Settings::on_pushButton_4_clicked() // настройки электрометра
{
    Settings_electrometer * setEl = new Settings_electrometer(this);
    setEl->setModal(true);
    setEl->show();
}

void Settings::on_pushButton_2_clicked() //настройки БДКН
{
    Settings_BDKN * setBDKN=new Settings_BDKN(this);
    setBDKN->setModal(true);
    setBDKN->show();
}

void Settings::on_pushButton_3_clicked() //настройки БДГ
{
    Settings_BDG * setBDG = new Settings_BDG(this);
    setBDG->setModal(true);
    setBDG->show();
}

void Settings::on_pushButton_clicked() //настройки пользователей
{
    user_settings* userSet = new user_settings(this);
    userSet->setModal(true);
    userSet->show();
}
