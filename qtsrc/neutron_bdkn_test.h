#ifndef NEUTRON_BDKN_TEST_H
#define NEUTRON_BDKN_TEST_H

#include "block_bdg.h"

#include <QDialog>

namespace Ui {
class Neutron_BDKN_test;
}

class Neutron_BDKN_test : public QDialog
{
    Q_OBJECT

public:
    explicit Neutron_BDKN_test(QWidget *parent = nullptr);
    ~Neutron_BDKN_test();

private slots:
    void on_pushButton_clicked();
    void errorConnectionSlot();
    void testPassedSlot();

private:
    Ui::Neutron_BDKN_test *ui;
    Block_BDG* bdg{nullptr};
    void init();
    void test();

    bool Passed=true;
};

#endif // NEUTRON_BDKN_TEST_H
