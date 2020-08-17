#ifndef VERIFYBETACHAMBERS_H
#define VERIFYBETACHAMBERS_H

#include <QDialog>
#include <QMessageBox>
#include <QDate>

#include "password.h"

namespace Ui {
class VerifyBetaChambers;
}

class VerifyBetaChambers : public QDialog
{
    Q_OBJECT

public:
    explicit VerifyBetaChambers(QWidget *parent = nullptr);
    ~VerifyBetaChambers();

    void setBut(int);
    void setNameCh(QString);

signals:
    void addRefreshSignal();

private slots:
    void on_pushButton_2_clicked();

    void on_lineEdit_4_textEdited(const QString &arg1);

    void on_pushButton_clicked();

private:
    Ui::VerifyBetaChambers *ui;
    void init();
    bool ButStatus{1};  // меняется в зависимости была нажата кнопка добавить =0  или правка =1;
    QString nameCh{""};
};

#endif // VERIFYBETACHAMBERS_H
