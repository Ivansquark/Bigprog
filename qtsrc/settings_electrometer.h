#ifndef SETTINGS_ELECTROMETER_H
#define SETTINGS_ELECTROMETER_H

#include "password.h"

#include <QDialog>


namespace Ui {
class Settings_electrometer;
}

class Settings_electrometer : public QDialog
{
    Q_OBJECT

public:
    explicit Settings_electrometer(QWidget *parent = nullptr);
    ~Settings_electrometer();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::Settings_electrometer *ui;
    void init();
};

#endif // SETTINGS_ELECTROMETER_H
