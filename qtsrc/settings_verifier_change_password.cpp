//******************** Изменение пароля поверителя ***********************************

#include "settings_verifier_change_password.h"
#include "ui_settings_verifier_change_password.h"


// QString newPassword{""};  - в этом месте создается глобальный объект, обычно так не делают
//(т.к. объект будет существовать до конца работы программы) обычно все объявляется в объявлении класса

settings_verifier_change_password::settings_verifier_change_password(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::settings_verifier_change_password)
{
    ui->setupUi(this);
    init();
}

settings_verifier_change_password::~settings_verifier_change_password()
{
    delete ui;
}


void settings_verifier_change_password::on_pushButton_2_clicked() // Назад
{
     close();
}

void settings_verifier_change_password::init()
{
    ui->pushButton->setEnabled(true); // Дополнительная проверка, что именно поверитель меняет пароль
    setWindowTitle("Изменение пароля поверителя");
}

void settings_verifier_change_password::on_lineEdit_2_editingFinished() //Строка нового пароля
{
    QString newPass1 = ui->lineEdit_2->text();
    newPassword = newPass1;
}

void settings_verifier_change_password::on_pushButton_clicked() // Применить
{
     QString newPass2 = ui->lineEdit_3->text();
     if((newPassword==newPass2)&&(newPass2!=""))
     {
             QSqlDatabase db=QSqlDatabase::database("myDB");
             db.setDatabaseName("config.db");
             if(!db.open()){qDebug()<<"db was not opened";}
             QSqlQuery query(db);

             QString queryStr = "UPDATE password SET passVerifier='%1'";
             QString queryStr1 = queryStr.arg(ui->lineEdit_3->text());
             if(!query.exec(queryStr1))
             {qDebug()<<"unable execute query UPDATE"<<query.lastError().text();}
             db.close();
             QMessageBox::information(this, "Внимание","Пароль изменен");
             close();
     }
    else
     {
         QMessageBox::warning(this, "Ошибка","Некорректный ввод");
     }
}
