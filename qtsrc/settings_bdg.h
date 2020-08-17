#ifndef SETTINGS_BDG_H
#define SETTINGS_BDG_H

#include <QDialog>

namespace Ui {
class Settings_BDG;
}

class Settings_BDG : public QDialog
{
    Q_OBJECT

public:
    explicit Settings_BDG(QWidget *parent = nullptr);
    ~Settings_BDG();

private slots:
    void on_pushButton_2_clicked();

private:
    Ui::Settings_BDG *ui;
};

#endif // SETTINGS_BDG_H
