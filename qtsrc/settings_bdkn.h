#ifndef SETTINGS_BDKN_H
#define SETTINGS_BDKN_H

#include <QDialog>

namespace Ui {
class Settings_BDKN;
}

class Settings_BDKN : public QDialog
{
    Q_OBJECT

public:
    explicit Settings_BDKN(QWidget *parent = nullptr);
    ~Settings_BDKN();

private slots:
    void on_pushButton_2_clicked();

private:
    Ui::Settings_BDKN *ui;
};

#endif // SETTINGS_BDKN_H
