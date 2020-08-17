#ifndef SETTINGS_VERIFIER_BETA_H
#define SETTINGS_VERIFIER_BETA_H

#include <QDialog>
#include <QSql>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>

#include "verifybetachambers.h"
#include "password.h"

namespace Ui {
class Settings_verifier_beta;
}

class Settings_verifier_beta : public QDialog
{
    Q_OBJECT

public:
    explicit Settings_verifier_beta(QWidget *parent = nullptr);
    ~Settings_verifier_beta();

private slots:
    void on_pushButton_4_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void addRefreshSlot();

    void on_pushButton_3_clicked();

private:
    Ui::Settings_verifier_beta *ui;
    void init();

};

#endif // SETTINGS_VERIFIER_BETA_H
