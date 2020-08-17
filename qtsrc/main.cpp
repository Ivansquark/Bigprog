#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString col="QWidget{color: white; background-color: rgb(80,78,77);}"

                    "QPushButton{font-size: 19px;"
                        "background-color:qradialgradient(cx:0.5, cy:0.5, radius: 1,"
                        "fx:0.5, fy:0.5, stop:0 rgba(188, 198, 204, 160), stop:1 rgba(240, 240, 240, 200));border:1px;border-radius:10px;"
                        "border:solid grey;"
                        "border-style: outset;"
                        "border-width: 3px;"
                        "border-color: (188, 198, 204, 100);"
                        "padding: 6px;}"
                        "QPushButton:pressed{border-radius:10px; background: qradialgradient(cx:0.5, cy:0.5, radius: 1,"
                        "fx:0.5, fy:0.5, stop:0 rgba(220, 220, 0, 150), stop:1 rgba(250, 250, 250, 150));border:1px;"
                        "border:solid grey;"
                        "border-style: outset;"
                        "border-width: 1px;"
                        "border-color: (188, 198, 204, 200);"
                        "padding: 6px;}"
                    "QLineEdit{color:black; background-color:white; min-height:30px;font-size:28px;}"
                    "QLabel{font-size:20px;}"
                    "QListWidget{font-size:30px}"
                    "QScrollBar::vertical{width: 40px}"
                    "QRadioButton::indicator{width: 25px; height: 25px;}"
                    "QRadioButton{font-size:24px;}"
                    "QComboBox{height:27px;font-size:25px;}"
                    "QCheckBox{height:25px;width:20px;font-size:25px;}"
                    "QTextEdit{font-size:20px;}";

            a.setStyleSheet(col);

    MainWindow *w=new MainWindow();
    w->move(0,0);
    w->showFullScreen();                        //showFullScreen() сделаем в самом конце, иначе не видно консоль отладки

    return a.exec();
}
