#ifndef ELECTROMETER_TEST_H
#define ELECTROMETER_TEST_H

#include "password.h"
#include "averageadc.h"
#include "electrometer_test_result.h"

#include <cmath>
#include <QDialog>
#include <QTimer>
#include <QMessageBox>

namespace Ui {
class Electrometer_test;
}

class Electrometer_test : public QDialog
{
    Q_OBJECT

signals:
    void sendToCom(QByteArray data);

public:
    explicit Electrometer_test(QWidget *parent = nullptr);
    ~Electrometer_test();

    void testSource(bool); //метод в который передаем флаг отвечающий за то из какого окна был вызван тест

signals:
    //void closeSignal();
    void closeTestAll();

public slots:
    void onCloseSlot();
    void testOkSlot();

private slots:
    void on_pushButton_clicked();
    void timeOut();
    void SlotGetADC();

private:
    Ui::Electrometer_test *ui;
    AverageADC* avADC{nullptr};
    QTimer* timer{nullptr};
    QTimer* timerGetADC{nullptr};
    QMessageBox* msg{nullptr};
    Electrometer_test_result* elTestRes{nullptr};

    void init();
    void delay(uint); //метод обеспечивающий задержку (отправляем в СОМ порт по два байта команд с задержкой между ними, т.к. по одному байту иногда не воспринимает электрометр)

    bool test_source{false}; //переменная хранящая в себе флаг отвечающий за то откуда был вызван тест
    bool testOk{false};     //переменная хранящая в себе флаг того что тест был пройден (или нет)
    long averageADC{0};
    quint8 heatingTime{0}; //время прогрева после установления связи с электрометром
    quint8 timeoutTest{0}; //время на переключение между режимами теста
    double R1{0},R2{0},R3{0}; //переменные в которые записываются значения резисторов
    double U_Ioff{0};         //переменная в которое записывается утечка в размерности разрядов АЦП (для последующей постоянной компенсации)
    uint DAC1{0};             //переменная в которую записывается вычисленное значение ЦАПа
    volatile double ADC_V{0};
    QByteArray bytesDAC;

    const uint ADC0 = 0x7fffff;
    QByteArray h00, // код реле - выкл все
               h21, // код реле - К5+К9 - замыкается 1 МОм и охранный с измерительным электроды
               h29, // код реле - К4+К5+К9 - замыкается 1 МОм + охранный с измерительным электроды + ИОН на 100 МОм(для калибровки 100 МОм)
               h15, // код реле - К2+К3+К9 - замыкается 100 МОм + охранный с измерительным электроды + ИОН на 10 ГОм(для калибровки 10 ГОм)
               h03, // код реле - К1+К9 - замыкается 10 ГОм + охранный с измерительным электроды - измерение входного тока
               hC2, // код реле - К1+К7+К8 - Чувствительный диапазон + фильтр шумов (С1 - 560пФ) (будем использовать всегда)
               h82, // код реле - К1+К8 - Чувствительный диапазон (не используется)
               h90, // код реле - К3+К8 - Средний диапазон (на среднем диапазоне фильтр шумов не подключается)
               hA0; // код реле - К5+К7+К8 - Грубый диапазон + фильтр шумов (С1 - 560пФ) (на грубом диапазоне фильтр шумов не отключается)
                    // + сброс заряда (по кнопке стоп) в режиме измерения заряда на чувствительном диапазоне

    //const QByteArray h21[2]={0x22,0x31};
    //const char h00[2]={0x20,0x30},
    //h21[2]={0x22,0x31},
    //h29[2]={0x22,0x39},
    //h15[2]={0x21,0x35},
    //h03[2]={0x20,0x33},
    //hC2[2]={0x2C,0x32},
    //h82[2]={0x28,0x32},
    //h90[2]={0x29,0x30},
    //hA0[2]={0x2A,0x30};


    double comCheck{0};

    long T_ADC{0},        //
         time{0},         //время
         i{0},            //
         x{0},            //переменная для рассчета среднего значения от ADC
         y{0},            //переменная для рассчета среднего среднего значения от ADC
         l1{0},           //вспомогательная переменная для расчета начального значения
         l2{0},           //вспомогательная переменная для расчета конечного значения
         averCounts{0};   //количество измерений (по которому усредняется)

    double averADC_V{0},
           ADC_offset{0},   // величина смещения при окончательной настройке ЦАПа
           d{0},            //вспомогательная переменная для расчета и записи значения ЦАПа
           I{0};            //вспомогательная переменная для расчета и записи значения тока смещения

};

#endif // ELECTROMETER_TEST_H
