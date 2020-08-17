#ifndef USER_SETTINGS_H
#define USER_SETTINGS_H

#include "settings_verifier_change_password.h"
#include <QDialog>

namespace Ui {
class user_settings;
}

class user_settings : public QDialog
{
    Q_OBJECT

public:
    explicit user_settings(QWidget *parent = nullptr);
    ~user_settings();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::user_settings *ui;
    void init();
};

#endif // USER_SETTINGS_H
