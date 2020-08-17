#ifndef VERIFYGAMMACHAMBERS_H
#define VERIFYGAMMACHAMBERS_H

#include <QDialog>
#include <QDate>
#include <QSql>
#include <QSqlQuery>
#include <QDebug>
#include <QMessageBox>
#include <QSqlError>

namespace Ui {
class VerifyGammaChambers;
}

class VerifyGammaChambers : public QDialog
{
    Q_OBJECT

public:
    explicit VerifyGammaChambers(QWidget *parent = nullptr);
    ~VerifyGammaChambers();

    void setBut(int);
    void setNameCh(QString);

signals:
    void addRefreshSignal();


private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_lineEdit_4_textEdited(const QString &arg1);

    void on_lineEdit_9_textEdited(const QString &arg1);

    void on_lineEdit_10_textEdited(const QString &arg1);

    void on_lineEdit_11_textEdited(const QString &arg1);

    void on_lineEdit_12_textEdited(const QString &arg1);

    void on_lineEdit_13_textEdited(const QString &arg1);

private:
    Ui::VerifyGammaChambers *ui;
    bool ButStatus{1}; //0-добавить, 1- удалить
    void init();
    QString nameCh{""};
};

#endif // VERIFYGAMMACHAMBERS_H
