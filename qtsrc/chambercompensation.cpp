//******** окно компенсации токов утечки электрометра ***********
#include "chambercompensation.h"
#include "ui_chambercompensation.h"

ChamberCompensation::ChamberCompensation(QWidget *parent,int voltage):
    QDialog(parent),
    ui(new Ui::ChamberCompensation),voltage(voltage)
{
    ui->setupUi(this);
    init();
}

ChamberCompensation::~ChamberCompensation()
{
    delete ui;
}

void ChamberCompensation::init()
{
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle("Компенсация");
    bool CurrSource=false;
    if(QSqlDatabase::contains("myDB"))
    {
        QSqlDatabase db=QSqlDatabase::database("myDB");
        db.setDatabaseName("config.db");
        if(!db.open()){qDebug()<<"db was not opened";}
        QSqlQuery query(db);
        QString qStr;
        //********************************************************************** определяем эмулятор или детектор
        qStr = "SELECT *FROM SourceChoice";
        if(!query.exec(qStr)){qDebug()<<"unable execute query SELECT table SourceChoice"<<query.lastError().text();}
        if(query.first())
        {
            CurrSource = static_cast<bool>(query.value("emulator").toUInt());
        }
        db.close();
    }
    qDebug()<<CurrSource;
    if(CurrSource){
        connect(this,&ChamberCompensation::exit,this,&ChamberCompensation::on_pushButton_clicked);
        emit exit();return;} //если эмулятор выйти - нечего тут делать
}

void ChamberCompensation::on_pushButton_clicked() //назад
{
    this->close();
    //this->destroy(); // почемуто сразу не выходил непонятно - какойто глюк

    qDebug()<<"on_pushButton_clicked";
}



void ChamberCompensation::on_pushButton_2_clicked() //кнопка выполнить компенсацию
{
    chamView = new CamberCompensation_view(this,voltage);
    connect(chamView,&CamberCompensation_view::compPar,this,&ChamberCompensation::compensationSlot);
    chamView->setModal(true);
    chamView->show();
}

void ChamberCompensation::compensationSlot(long ucomp1,long ucomp2,long ucomp3)
{
    Ucomp1=ucomp1;Ucomp2=ucomp2;Ucomp3=ucomp3;

    if(QSqlDatabase::contains("myDB"))
    {
        QSqlDatabase db = QSqlDatabase::database("myDB");
        db.setDatabaseName("config.db"); //подключаемся к файлу config.db, если его нет создается новый файл базы данных с именем config.db (можно удалить тогда создастся заново)

        if(!db.open()) //открываем БД, если не открылась пишем предупреждение в консоль
        {        qDebug()<<"DB was not opened"<<db.lastError().text();  }

        QSqlQuery query(db);// создаем объект через который будут идти запросы к БД, инициализируем созданным объектом БД (db)
        QString queryStr1 = "UPDATE chamberCompensation SET  Comp1='%1',Comp2='%2',Comp3='%3';";
        QString queryStr=queryStr1.arg(QString::number(Ucomp1))
                                  .arg(QString::number(Ucomp2))
                                  .arg(QString::number(Ucomp3));

        if(!query.exec(queryStr))
        {qDebug()<<"unable execute query UPDATE"<<query.lastError().text();}
        db.close();
    }
    ui->label->setText(QString::number(Ucomp1*0.0000003,'g',7));
    ui->label_2->setText(QString::number(Ucomp2*0.0000003,'g',7));
    ui->label_3->setText(QString::number(Ucomp3*0.0000003,'g',7));

    if(Ucomp1!=0&&Ucomp2!=0&&Ucomp3!=0) //если все три параметра не равны нулю (что говорит опроведенной конпенсации)
    {
        emit compensationFlagSig(); //посылаем сигнал о проведенной компенсации
    }
}

//void ChamberCompensation::on_pushButton_3_clicked()
//{
//    if(QSqlDatabase::contains("myDB"))
//    {
//        QSqlDatabase db = QSqlDatabase::database("myDB");
//        db.setDatabaseName("config.db"); //подключаемся к файлу config.db, если его нет создается новый файл базы данных с именем config.db (можно удалить тогда создастся заново)
//
//        if(!db.open()) //открываем БД, если не открылась пишем предупреждение в консоль
//        {        qDebug()<<"DB was not opened"<<db.lastError().text();  }
//
//        QSqlQuery query(db);// создаем объект через который будут идти запросы к БД, инициализируем созданным объектом БД (db)
//
//
//        //***************************************************************************************************************
//        QString queryStr = "CREATE TABLE IF NOT EXISTS chamberCompensation (id INTEGER PRIMARY KEY CHECK (id=0),"  //создаем новую таблицу из одной строки т.к (id=0)
//                                                                 "Comp1 TEXT(10),"                          //в которую заводим три колонки с текстовым содержимым
//                                                                 "Comp2 TEXT(10),"                      //пароль поверителя, пароль изготовителя
//                                                                 "Comp3 TEXT(10))";                          //текущуй пароль
//        //********************************************************************************************************************
//
//        if(!query.exec(queryStr)) //передаем строку с указанием создать таблицу в метод exec() объекта query
//        {qDebug()<<"unable execute query CREATE"<<query.lastError().text();}
//        queryStr = "INSERT INTO chamberCompensation (id,Comp1, Comp2, Comp3) VALUES (0,'0','0','0')"; //Вставляем в таблицу строку (вставить можно только одну строку)
//        if(!query.exec(queryStr))
//        {qDebug()<<"unable execute query INSERT"<<query.lastError().text();}
//        db.close();
//    }
//}
