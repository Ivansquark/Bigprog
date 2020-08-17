#ifndef PASSWORD_H
#define PASSWORD_H

#include <QString>
#include <QSql>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>


class Password
{
public:
    Password();
    static int check();
};

#endif // PASSWORD_H
