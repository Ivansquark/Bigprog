#ifndef SETTINGS_VERIFIER_GAMMA_BDG_H
#define SETTINGS_VERIFIER_GAMMA_BDG_H

#include "password.h"

#include <QDialog>
#include <QDate>

namespace Ui {
class Settings_verifier_gamma_bdg;
}

class Settings_verifier_gamma_bdg : public QDialog
{
    Q_OBJECT

public:
    explicit Settings_verifier_gamma_bdg(QWidget *parent = nullptr);
    ~Settings_verifier_gamma_bdg();

    void setBut(bool);
    void setNameCh(QString str);

signals:
    void addRefreshSignal();

private slots:
    void on_pushButton_4_clicked();

    void on_lineEdit_17_textEdited(const QString &arg1);

    void on_lineEdit_18_textEdited(const QString &arg1);

    void on_lineEdit_19_textEdited(const QString &arg1);

    void on_lineEdit_20_textEdited(const QString &arg1);

    void on_lineEdit_21_textEdited(const QString &arg1);

    void on_lineEdit_22_textEdited(const QString &arg1);

    void on_pushButton_3_clicked();

private:
    Ui::Settings_verifier_gamma_bdg *ui;
    bool ButStatus{1};
    QString nameCh{""};
    void init();
};

#endif // SETTINGS_VERIFIER_GAMMA_BDG_H
