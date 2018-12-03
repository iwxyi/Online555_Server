#ifndef USERLIST_H
#define USERLIST_H

#include <QObject>
#include <QString>
#include <QList>
#include <QStringList>
#include "globals.h"

class User
{
public:
    User(QString username, QString password, int integral) :
        username(username), password(password), integral(integral) { }

    User(QString userID, QString username, QString password, int integral) :
        userID(userID), username(username), password(password), integral(integral) { }

    QString userID;
    QString username;
    QString password;
    int integral;
};

class UserList
{
public:
    UserList();

    int count();
    int readFromFile();
    bool writeToFile();
    void addIntegral(QString userID, int addition);
    bool canRigister(QString n);
    QString exists(QString n, QString p);
    QString getRandomUserID();
    int getIntegral(QString userID);
    int getRank(QString userID);
    QString getRanks();
    QString add(QString n, QString p);

private:
    QList<User>users; // 用户信息列表（不论顺序，会按照积分来排序）
    QString filePath;
};

#endif // USERLIST_H
