//*******************************  Нейтронный блок - тест оборудования  **********************

#include "neutron_bdkn_test.h"
#include "ui_neutron_bdkn_test.h"

Neutron_BDKN_test::Neutron_BDKN_test(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Neutron_BDKN_test)
{
    ui->setupUi(this);
    init();
}

Neutron_BDKN_test::~Neutron_BDKN_test()
{
    delete ui;
}

void Neutron_BDKN_test::init()
{
    setWindowTitle("Нейтронный блок - тест оборудования");
    setAttribute(Qt::WA_DeleteOnClose); //обычно вызывается deleteLater, данный параметр вызывает деструктор при закрытии окна
    ui->label->setText("");
    test();
    QTimer::singleShot(2000,this,SLOT(testPassedSlot()));
}

void Neutron_BDKN_test::test()
{
    bdg = new Block_BDG;//создается объект блока БДГ (в него передаются соответствующие ему параметры)
    connect(bdg,&Block_BDG::errorSig,this,&Neutron_BDKN_test::errorConnectionSlot);
}
void Neutron_BDKN_test::errorConnectionSlot()
{
    //QMessageBox::warning(this, "Ошибка","Ошибка в подключении");
    //close();
    Passed=false;
    ui->label->setText("Тест не пройден, ошибка подключения");
}

void Neutron_BDKN_test::testPassedSlot()
{
    if(Passed)
    {ui->label->setText("Тест успешно пройден");}
}


void Neutron_BDKN_test::on_pushButton_clicked() // отмена
{
    close();
}


