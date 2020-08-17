#ifndef CAMBERCOMPENSATION_VIEW_H
#define CAMBERCOMPENSATION_VIEW_H

#include "averageadc.h"
#include "volthex.h"

#include <QDialog>
#include "QTimer"

namespace Ui {
class CamberCompensation_view;
}

class CamberCompensation_view : public QDialog
{
    Q_OBJECT

public:
    explicit CamberCompensation_view(QWidget *parent = nullptr,int voltage = 0);
    ~CamberCompensation_view();

signals:
    void compPar(double,double,double);

private slots:
    void on_pushButton_clicked();
    void timeOut();
    void delayTimeOut();

private:
    Ui::CamberCompensation_view *ui;
    AverageADC* averageADC{nullptr};
    QTimer* timer{nullptr};
    QTimer* delayTimer{nullptr};

    void init();
    void delay(int x);

    int voltage{0};
    uint time{0};
    long ADC{0};
    double ADC_V{0};
    long Comp1{0},Comp2{0},Comp3{0};
    int y{0};
    const uint ADC0{0x7FFFFF};
    uint Ioff{0};


};

#endif // CAMBERCOMPENSATION_VIEW_H
