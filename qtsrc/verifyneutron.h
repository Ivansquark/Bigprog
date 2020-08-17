#ifndef VERIFYNEUTRON_H
#define VERIFYNEUTRON_H

#include <QDialog>
#include <QDate>
#include <QSql>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include <QMessageBox>

namespace Ui {
class VerifyNeutron;
}

class VerifyNeutron : public QDialog
{
    Q_OBJECT

public:
    explicit VerifyNeutron(QWidget *parent = nullptr);
    ~VerifyNeutron();

    void setBut(int x);
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
    Ui::VerifyNeutron *ui;
    void init();
    bool ButStatus{1};
    QString nameCh{""};
};

#endif // VERIFYNEUTRON_H
