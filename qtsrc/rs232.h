#ifndef RS232_H
#define RS232_H


#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QTimer>
#include <QDebug>

class RS232:public QObject
{
    Q_OBJECT
public:
    explicit RS232(QObject* parent=nullptr);
    ~RS232();

signals:
    void outRS232(QString);

private slots:
    void timeOut();
    void startTimeOut();

public:
    void writeToRS232(QByteArray&);


private:
    QSerialPort* comPort{nullptr};
    QString COM;
    QTimer* timer{nullptr};
    QTimer* startTimer{nullptr};

    void init();
    void connectPort();
    void disconnectPort();


};

#endif // RS232_H
