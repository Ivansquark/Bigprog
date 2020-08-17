#ifndef CHAMBERCOMPENSATION_H
#define CHAMBERCOMPENSATION_H

#include "password.h"
#include "cambercompensation_view.h"

#include <QDialog>

namespace Ui {
class ChamberCompensation;
}

class ChamberCompensation : public QDialog
{
    Q_OBJECT

public:
    explicit ChamberCompensation(QWidget *parent = nullptr,int voltage=0);
    ~ChamberCompensation();

signals:
    void exit();
   void compensationFlagSig();


public slots:
   void compensationSlot(long,long,long);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

//    void on_pushButton_3_clicked();

private:
    Ui::ChamberCompensation *ui;
    CamberCompensation_view* chamView{nullptr};

    void init();

    int voltage{0};
    long Ucomp1{0};
    long Ucomp2{0};
    long Ucomp3{0};
};

#endif // CHAMBERCOMPENSATION_H
