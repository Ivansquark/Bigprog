//********************************* Гамма излучение - БДГ - тест оборудования *****************************

#include "gamma_bdg_test.h"
#include "ui_gamma_bdg_test.h"

Gamma_BDG_test::Gamma_BDG_test(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Gamma_BDG_test)
{
    ui->setupUi(this);
    init();
}

Gamma_BDG_test::~Gamma_BDG_test()
{
    delete bdg;
    qDebug()<<"~Gamma_BDG_test()";
}

void Gamma_BDG_test::init()
{
    setWindowTitle("Гамма излучение - БДГ - тест оборудования");
    this->setAttribute(Qt::WA_DeleteOnClose); //обычно вызывается deleteLater, данный параметр вызывает деструктор при закрытии окна
    ui->label->setText("");
    test();
    QTimer::singleShot(2000,this,SLOT(testPassedSlot()));
}

void Gamma_BDG_test::test()
{
    bdg = new Block_BDG;//создается объект блока БДГ (в него передаются соответствующие ему параметры)
    connect(bdg,&Block_BDG::errorSig,this,&Gamma_BDG_test::errorConnectionSlot);
}
void Gamma_BDG_test::errorConnectionSlot()
{
    //QMessageBox::warning(this, "Ошибка","Ошибка в подключении");
    //close();
    Passed=false;
    ui->label->setText("Тест не пройден, ошибка подключения");
}

void Gamma_BDG_test::testPassedSlot()
{
    if(Passed)
    {ui->label->setText("Тест успешно пройден");}
}

void Gamma_BDG_test::on_pushButton_clicked() //Отмена
{    
    close();
}

