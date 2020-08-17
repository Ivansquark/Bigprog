#ifndef GAMMA_BDG_TEST_H
#define GAMMA_BDG_TEST_H

#include "modbus_emulator.h"
#include "averagemodbus.h"
#include "iblock.h"
#include "rs485.h"

#include "electrometer_test_result.h"
#include "block_bdg.h"

#include <QDialog>
#include <QThread>

namespace Ui {
class Gamma_BDG_test;
}

class Gamma_BDG_test : public QDialog
{
    Q_OBJECT

public:
    explicit Gamma_BDG_test(QWidget *parent = nullptr);
    ~Gamma_BDG_test();

private slots:
    void on_pushButton_clicked();
    void errorConnectionSlot();
    void testPassedSlot();

private:
    Ui::Gamma_BDG_test *ui;    
    Block_BDG* bdg{nullptr};
    void init();
    void test();

    bool Passed=true;
};

#endif // GAMMA_BDG_TEST_H
