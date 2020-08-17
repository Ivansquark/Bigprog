#include "volthex.h"

voltHex::voltHex()
{

}

QByteArray voltHex::intVolt(int volt)
{
    if(volt<-2)
    {
        double i =4.98;
        double x = round(volt/i)*(-1);
        int y = static_cast<int>(x);
        QString hexV = QString::number(y,16);
        //qDebug()<<hexV;
        QString strHexV1 = "7"+hexV[0];
        QString strHexV2 = "8"+hexV[1];
        QByteArray byteVolt;
        byteVolt[0] = static_cast<char>(strHexV1.toUInt(nullptr,16));
        byteVolt[1] = static_cast<char>(strHexV2.toUInt(nullptr,16));

        //qDebug()<<QString::number(byteVolt[0],16)<<QString::number(byteVolt[1],16);
        return byteVolt; // возвращаем массив байтов.
    }
    else if(volt>=-2 && volt<0)
    {
        QByteArray byteVolt;
        byteVolt[0] = static_cast<char>(QString("70").toUInt(nullptr,16));
        byteVolt[1] = static_cast<char>(QString("80").toUInt(nullptr,16));
        //qDebug()<<QString::number(byteVolt[0],16)<<QString::number(byteVolt[1],16);
        return byteVolt; // возвращаем массив байтов.
    }
    else if(volt>=0 && volt<3)
    {
        QByteArray byteVolt;
        byteVolt[0] = static_cast<char>(QString("78").toUInt(nullptr,16));
        byteVolt[1] = static_cast<char>(QString("80").toUInt(nullptr,16));
        //qDebug()<<QString::number(byteVolt[0],16)<<QString::number(byteVolt[1],16);
        return byteVolt; // возвращаем массив байтов.
    }
    if(volt>=3)
    {
        double i =4.98;
        double x = round(volt/i)+128;
        int y = static_cast<int>(x);
        QString hexV = QString::number(y,16);
        //qDebug()<<hexV;
        QString strHexV1 = "7"+hexV[0];
        QString strHexV2 = "8"+hexV[1];
        QByteArray byteVolt;
        byteVolt[0] = static_cast<char>(strHexV1.toUInt(nullptr,16));
        byteVolt[1] = static_cast<char>(strHexV2.toUInt(nullptr,16));
        //qDebug()<<QString::number(byteVolt[0],16)<<QString::number(byteVolt[1],16);
        return byteVolt; // возвращаем массив байтов.
    }
    return nullptr;
}
