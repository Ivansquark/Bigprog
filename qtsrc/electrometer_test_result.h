#ifndef ELECTROMETER_TEST_RESULT_H
#define ELECTROMETER_TEST_RESULT_H

#include "password.h"

#include <QDialog>


namespace Ui {
class Electrometer_test_result;
}

class Electrometer_test_result : public QDialog
{
    Q_OBJECT

public:
    explicit Electrometer_test_result(QWidget *parent = nullptr);
    ~Electrometer_test_result();


signals:
    void onCloseSig(); //сигнал о закрытии предыдущего окна (или двух окон в зависимости где был открыт тест)
    void testOkSig();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::Electrometer_test_result *ui;
    void init();
    double R2{0},R3{0},I{0};
    bool test_source{false}; //переменная отвечающая за то откуда вызывался тест (0-из окна тест 1-из окна измерение)
    double DAC{0};
    double RefVolt{0};

};

#endif // ELECTROMETER_TEST_RESULT_H
