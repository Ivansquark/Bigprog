//*************************** Первое окно *********************************


#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)          //конструктор MainWindow (инициализируется QWidget,QMainWindow-родители и выделяется память для ui_mainwindow.h)
{
    ui->setupUi(this);  //указываем на то что создано в ui_mainwindow.h
    init();            //в конструкторе запускаем функцию (реализация далее) в которой происходит инициализация того что нам нужно
}

MainWindow::~MainWindow()   //деструктор
{
    delete ui; //очищаем память размером с объект ui по адресу ui
}

void MainWindow::on_pushButton_4_clicked()//exit
{
    close();
}

void MainWindow::init()
{
    setWindowTitle("Баскет-БГН");
    ui->pushButton_6->setEnabled(false);
    ui->pushButton_6->setVisible(false);//делаем кнопку настройки невидимой при загрузке (сделаем видимой после введения пароля изготовителя)
    ui->pushButton_7->setEnabled(false);
    ui->pushButton_7->setVisible(false);//делаем кнопку настройки невидимой при загрузке (сделаем видимой после введения пароля поверителя или изготовителя)
    QSqlDatabase::addDatabase("QSQLITE","myDB");    //загружаем драйвер БД(SQLITE) и создаем имя подключения к БД
    QSqlDatabase db=QSqlDatabase::database("myDB"); //создаем объект БД и присваиваем ему имя подключения
    db.setDatabaseName("config.db"); //подключаемся к файлу config.db, если его нет создается новый файл базы данных с именем config.db (можно удалить тогда создастся заново)

    if(!db.open()) //открываем БД, если не открылась пишем предупреждение в консоль
    {        qDebug()<<"DB was not opened"<<db.lastError().text();  }

    QSqlQuery query(db);// создаем объект через который будут идти запросы к БД, инициализируем созданным объектом БД (db)


    //***************************************************************************************************************
    QString queryStr = "CREATE TABLE IF NOT EXISTS password (id INTEGER PRIMARY KEY CHECK (id=0),"             //создаем новую таблицу из одной строки т.к (id=0)
                                                             "passVerifier TEXT(10),"                          //в которую заводим три колонки с текстовым содержимым
                                                             "passManufacturer TEXT(10),"                      //пароль поверителя, пароль изготовителя
                                                             "passCurrent TEXT(10))";                          //текущуй пароль
    //********************************************************************************************************************

    if(!query.exec(queryStr)) //передаем строку с указанием создать таблицу в метод exec() объекта query
    {qDebug()<<"unable execute query CREATE"<<query.lastError().text();}
    //queryStr = "INSERT INTO password (id,passVerifier, passManufacturer, passCurrent) VALUES (0,'КЕРМА','777','1')"; //Вставляем в таблицу строку (вставить можно только одну строку)
    //if(!query.exec(queryStr))
    //{qDebug()<<"unable execute query INSERT"<<query.lastError().text();}
    queryStr = "UPDATE password SET  passCurrent='1';";  // при загрузке меняем passCurrent на '1', чтобы по умолчанию загружался оператор (а не поверитель и изготовитель)
    if(!query.exec(queryStr))
    {qDebug()<<"unable execute query UPDATE"<<query.lastError().text();}
    db.close();
}

void MainWindow::on_pushButton_5_clicked() //кнопка проверить
{
    QString passCurrent{"1"}; //переменная в которую запишется текущий введенный пароль
    QString passManufacturer{""};
    QString passVerifier{""};
    //passCurrent = ui->lineEdit->text();
    if(QSqlDatabase::contains("myDB"))
    {
        QSqlDatabase db = QSqlDatabase::database("myDB");
        db.setDatabaseName("config.db");
        if(!db.open()) {qDebug()<<"db was not opened";}
        QSqlQuery query(db);
        QString queryStr = "UPDATE password SET passCurrent='%1'"; //изменяем passCurrent в таблице password
        QString queryStr1 = queryStr.arg(ui->lineEdit->text());    //вставляем вместо '%1' введенную строку
        if(!query.exec(queryStr1))
        {qDebug()<<"unable execute query UPDATE"<<query.lastError().text();}

        queryStr = "SELECT * FROM password";
        if(!query.exec(queryStr))                    //выполнить запрос на выбор всего(*) из таблицы password
        {qDebug()<<"unable execute query SELECT"<<query.lastError().text();}
        if(query.first()) // обращение к первой строке таблицы
        {
            passCurrent=query.value("passCurrent").toString();
            passManufacturer=query.value("passManufacturer").toString();
            passVerifier=query.value("passVerifier").toString();
        }
    }
    if(passCurrent==passManufacturer)       // проверяем введенный пароль
    {
        ui->pushButton_6->setEnabled(true);
        ui->pushButton_6->setVisible(true);     //делаем кнопку видимой
        ui->pushButton_7->setEnabled(true);
        ui->pushButton_7->setVisible(true);
        ui->label_2->setText("ИЗГОТОВИТЕЛЬ");
    }
    else if (passCurrent==passVerifier)
    {
        ui->pushButton_6->setEnabled(false);
        ui->pushButton_6->setVisible(false);
        ui->pushButton_7->setEnabled(true);
        ui->pushButton_7->setVisible(true);
        ui->label_2->setText("ПОВЕРИТЕЛЬ");
    }
    else
    {
        ui->pushButton_6->setEnabled(false);
        ui->pushButton_6->setVisible(false);
        ui->pushButton_7->setEnabled(false);
        ui->pushButton_7->setVisible(false);
        ui->label_2->setText("ОПЕРАТОР");
    }
}

void MainWindow::on_pushButton_6_clicked() //кнопка настройки
{
    Settings *settings = new Settings(this);    //передаем в конструктор Settings указатель this на текущий объект класса MainWindow (делается для того, чтобы при закрытии объекта MainWindow автоматически очищалась память выделенная в объекте Settings)
    settings->setModal(true);                   //делаем окно модальным, чтобы нельзя было нажать нигде в программе кроме него
    settings->show();                           //showFullScreen() сделаем в самом конце, иначе не видно консоль отладки
}

void MainWindow::on_pushButton_7_clicked() //настройки поверителя
{
    Settings_verifier *setVer = new Settings_verifier(this);
    setVer->setModal(true);
    setVer->show();
}

void MainWindow::on_pushButton_clicked() //бета
{
    Beta * beta = new Beta(this);
    beta->setModal(true);
    beta->show();
}

void MainWindow::on_pushButton_2_clicked() //гамма
{
    Gamma * gamma = new Gamma(this);
    gamma->setModal(true);
    gamma->show();
}

void MainWindow::on_pushButton_3_clicked() // нейтроное
{
    Neutron * neutron = new Neutron(this);
    neutron->setModal(true);
    neutron->show();
}
