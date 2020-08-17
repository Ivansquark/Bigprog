#ifndef EMULELECTROMETER_H
#define EMULELECTROMETER_H

#include <QObject>
#include <QTimer>
#include <QDebug>

class EmulElectrometer:public QObject
{
    Q_OBJECT
public:
    EmulElectrometer();
    ~EmulElectrometer();

signals:
    void sendd(QString str);

private slots:
    void timeOut();

private:
    QString comString{"273f4f5f6f7f\0"}; //инициализируем строку данными наподобие данных приходящих с АЦП электрометра
    QString sendString{""};
    QTimer *timer{nullptr};
    long iter=0;
public:
    void start();         //старт эмулятора
    void stop();          //стоп эмулятора
    //QString getString();
};

#endif // EMULELECTROMETER_H
