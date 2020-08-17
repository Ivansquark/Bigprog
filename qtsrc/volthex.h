#ifndef VOLTHEX_H
#define VOLTHEX_H

#include <QDialog>
#include <QDebug>

#include "math.h"


class voltHex: public QDialog
{
    Q_OBJECT
public:
    voltHex();


//    void byteVolts(QByteArray);


QByteArray  intVolt(int);

};

#endif // VOLTHEX_H
