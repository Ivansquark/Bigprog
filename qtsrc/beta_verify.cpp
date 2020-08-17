//**************************** калибровка камер бета излучения ********************************

#include "beta_verify.h"
#include "ui_beta_verify.h"

Beta_verify::Beta_verify(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Beta_verify)
{
    ui->setupUi(this);
    init();
}

Beta_verify::~Beta_verify()
{
    delete timer;
    delete startTimer;
    delete betaChamber;
    delete ui;
}

void Beta_verify::init()
{
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle("Калибровка камер бета излучения");
    ui->comboBox->addItem("Гр/с");
    ui->comboBox->addItem("мГр/с");
    ui->comboBox->addItem("Гр/ч");
    ui->comboBox->addItem("мГр/ч");
    ui->label->setStyleSheet("font-size:100px; color: red;");
    ui->label_2->setStyleSheet("font-size:100px; color: red;");
    ui->label_3->setStyleSheet("font-size:50px; color: cyan;");

    ui->lineEdit->setValidator(new QDoubleValidator(0.0, 1000.0, 5, ui->lineEdit));
    //только цифры, 5 знаков после запятой
    ui->lineEdit_2->setValidator(new QDoubleValidator(0.0, 1000.0, 5, ui->lineEdit_2));
    //только цифры, 5 знаков после запятой
    timer = new QTimer(this);
    connect(timer,&QTimer::timeout,this,&Beta_verify::timeOut);
    startTimer = new QTimer(this);
    connect(startTimer,&QTimer::timeout,this,&Beta_verify::startTimeOut);

    /*! <Окно с задержкой выполнения программы> */
    QMessageBox* startMsg = new QMessageBox(QMessageBox::Icon::Information,"","",QMessageBox::NoButton,this,Qt::FramelessWindowHint);
    startMsg->setText("Произодится конфигурирование оборудования");
    startMsg->setStandardButtons(0);
    startMsg->setStyleSheet("background-color: rgb(180,178,177);");
    QTimer::singleShot(4000,startMsg,SLOT(hide()));
    startMsg->exec();
    delete startMsg;
}

void Beta_verify::getBetaChamber(Beta_chamber *beta)
{
    betaChamber = beta;
    chamberName = betaChamber->getChamberName();
    ui->label_5->setText(chamberName);
    QMessageBox::information(this,"Внимание","Введите значение Мощности Поглощенной Дозы\n"
                                             "в даннной точке измерения и нажмите старт\n\n"
                                             "При нажатии кнопки стоп выведется значение чувствительности,"
                                             " которое необходимо записать",QMessageBox::Ok);
}

void Beta_verify::timeOut() //раз в секунду
{
    //ui->label->setText(QString::number(betaChamber->getIres(),'E'));

    //------------------------------------Время-------------------------------------------------------
        time++;
        ui->label_3->setText(QString::number(time) + " c"); //отображение времени
    //------------------------------------Мощность дозы-------------------------------------------------------
        I = betaChamber->getIres(); //считываем измерение из класса Beta_chamber в Гр/с
        //qDebug()<<I;

    //------------------------------------СКО и Среднее-------------------------------------------------------
        Isum+=I;
        avI=(Isum/time);// - средняя величина мощности дозы
        //double avDoseRate = dose*timeCoef;
        ui->label->setText(QString::number(avI,'E'));
        otkl+=I*I; //сумма квадратов случайной величины
        if(time>0)
        {
            double z=sqrt((otkl/time)-avI*avI); //CKO по хитрой формуле: корень из D=(1/N)SUM(1,N,Xi^2)-M^2
            // формула получена на основании того что СКО - это корень из дисперсии
            // Дисперсия - математическое ожидание квадрата отклонения случайной величины от её математического ожидания
            // D=M[X-M(X)]^2 = M(X^2) - M^2(X); - особенное свойство которое везде доказывается
            // M(X^2) - для равновероятных событий является = (1/N)SUM(1,N,doseRate*doseRate)
            // Дисперсия равняется мат ожиданию квадрата случайной величины минус квадрат мат ожидания случайной величины
            // отсюда и получается данная формула: корень из D=(1/N)SUM(1,N,Xi^2)-(среднее)^2

            StandardDeviation=std::abs(z/avI); //под СКО подразумеваем СКО деленное на среднее и умножить на 100% (относительное отклонение от среднего)
            ui->label_4->setText(QString::number(StandardDeviation*100,'f',3)+" %");
        }
}

void Beta_verify::startTimeOut()
{
    ui->pushButton_3->setEnabled(true);
    timer->start(1000);
}

void Beta_verify::on_pushButton_2_clicked() //кропка старт
{
    ui->pushButton_2->setEnabled(false);
    ui->pushButton->setEnabled(false);
    ui->pushButton_2->setVisible(false);
    ui->pushButton->setVisible(false);
    ui->pushButton_3->setEnabled(false);


    startTimer->setSingleShot(true);
    startTimer->start(60000);
    Isum=0;
    time=0;
    otkl=0;
    ui->label_3->setText("ПРОГРЕВ!");
}

void Beta_verify::on_pushButton_3_clicked() // кнопка стоп
{
    ui->pushButton_2->setEnabled(true);
    ui->pushButton->setEnabled(true);
    ui->pushButton_2->setVisible(true);
    ui->pushButton->setVisible(true);
    timer->stop();
    if(ui->label->text()=="Ток"||ui->lineEdit->text()==""){ui->lineEdit_2->setText("");}
    else
    {
        double I = avI;
        QString str=ui->lineEdit->text(); //считываем порог по дозе в переменную
        str.replace(",",".");  // меняем запятые на точки, чтобы корректно преобразовывался в double
        double doseRate = str.toDouble();
        //qDebug()<<"doseRate= "<<doseRate;
        double sens=0;
        switch (ui->comboBox->currentIndex()) //0-Гр/с 1-мГр/с 2-Гр/ч 3-мГр/ч
        {
            case 0: sens=std::abs(doseRate/I);break; // (Гр/с)/(Кл/с)=Гр/с
            case 1: sens=std::abs((1/(1E3))*doseRate/I);break; // (мГр/с)/(Кл/с)=Гр/с /(1000)
            case 2: sens=std::abs((1/(3.6E3))*doseRate/I);break; // (Гр/ч)/(Кл/с)=Гр/с  /(3600)
            case 3: sens=std::abs((1/(3.6E6))*doseRate/I);break; // (мГр/ч)/(Кл/с)=Гр/с /(3600000)
        default: break;
        }

        QString sensibility = QString::number(sens);
        ui->lineEdit_2->setText(sensibility);
    }
}

void Beta_verify::on_pushButton_4_clicked() //кнопка записать
{
    if (ui->lineEdit_2->text()==""){QMessageBox::warning(this,"Внимание","Не задано значение",QMessageBox::Ok);}
    else
    {
        QString sensibility = ui->lineEdit_2->text(); // Чувствительность в Зависимости от ComboBox-a
        if(QSqlDatabase::contains("myDB"))
        {
            QSqlDatabase db = QSqlDatabase::database("myDB");
            db.setDatabaseName("config.db");
            if(!db.open()){qDebug()<<"db was not opened";}
            QSqlQuery query(db);
            QString qStr="UPDATE betaChambers SET sensibility='%1' WHERE name='%2'";
            QString qStr1=qStr.arg(ui->lineEdit_2->text()).arg(chamberName);
            qDebug()<<chamberName;
            if(!query.exec(qStr1)){qDebug()<<"unable execute query UPDATE betaChambers";db.lastError().text();}
            db.close();
        }
        QMessageBox::warning(this,"Внимание","Значение чувствительности было изменено",QMessageBox::Ok);
    }
}

void Beta_verify::on_pushButton_clicked() //назад
{

    close();
}
