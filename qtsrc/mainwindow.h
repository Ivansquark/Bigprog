#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "beta.h"
#include "gamma.h"
#include "neutron.h"
#include "settings.h"
#include "settings_verifier.h"

#include <QMainWindow>      //подключаем класс от которого наследуемся
//#include <QSql>           //подключаем класс с параметрами SQL (работает и без него)
#include <QSqlDatabase>     //подключаем класс настроек для работы с БД
#include <QSqlQuery>        //подключаем класс обработки запросов к БД
#include <QSqlError>        //подключаем класс вывода ошибок SQL запросов
#include <QDebug>           //подключаем класс вывода отладочной информации в консоль

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow     // наследуется от QMainWindow
{
    Q_OBJECT    // включает механизм сигналов слотов

public:
    explicit MainWindow(QWidget *parent = nullptr);  //объявляется конструктор который является конструктором по умолчанию, т.к. сразу инициализирует объект
    ~MainWindow();                                   //объявляется деструктор

private slots:
    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;
    void init();    
};

#endif // MAINWINDOW_H
