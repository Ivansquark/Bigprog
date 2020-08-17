#ifndef SETTINGS_VERIFIER_CHANGE_PASSWORD_H
#define SETTINGS_VERIFIER_CHANGE_PASSWORD_H

#include "password.h"

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>



namespace Ui {
class settings_verifier_change_password;
}

class settings_verifier_change_password : public QDialog
{
    Q_OBJECT

public:
    explicit settings_verifier_change_password(QWidget *parent = nullptr);
    ~settings_verifier_change_password();

private slots:
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();
    void on_lineEdit_2_editingFinished();


private:
    Ui::settings_verifier_change_password *ui;

    void init();

    QString newPassword{""};

};

#endif // SETTINGS_VERIFIER_CHANGE_PASSWORD_H


