#ifndef SETTINGS_VERIFIER_GAMMA_NUCLIDES_H
#define SETTINGS_VERIFIER_GAMMA_NUCLIDES_H

#include "settings_verifier_gamma_bdg.h"

#include <QDialog>

namespace Ui {
class Settings_verifier_gamma_nuclides;
}

class Settings_verifier_gamma_nuclides : public QDialog
{
    Q_OBJECT

public:
    explicit Settings_verifier_gamma_nuclides(QWidget *parent = nullptr);
    ~Settings_verifier_gamma_nuclides();



private slots:
    void on_pushButton_4_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void addRefreshSlot();

    void on_pushButton_3_clicked();

private:
    Ui::Settings_verifier_gamma_nuclides *ui;
    void init();
};

#endif // SETTINGS_VERIFIER_GAMMA_NUCLIDES_H
