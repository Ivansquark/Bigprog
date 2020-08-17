#ifndef SETTINGS_H
#define SETTINGS_H

#include "settings_electrometer.h"
#include "settings_bdkn.h"
#include "settings_bdg.h"
#include "user_settings.h"

#include <QDialog>

namespace Ui {
class Settings;
}

class Settings : public QDialog
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = nullptr);
    ~Settings();

private slots:
    void on_pushButton_5_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_clicked();

private:
    Ui::Settings *ui;
    void init();
};

#endif // SETTINGS_H
