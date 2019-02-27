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

    int count(); // 用户数量
    int readFromFile(); // 从文件中读取
    bool writeToFile(); // 保存到文件
    void addIntegral(QString userID, int addition); // 为用户添加积分
    bool canRigister(QString n); // 该用户名能否注册
    QString exists(QString n, QString p); // 是否能用该昵称和密码登录
    QString getRandomUserID(); // 为新用户准备一个唯一的用户ID
    int getIntegral(QString userID); // 获取用户积分
    int getRank(QString userID); // 获取用户排名
    QString getRanks(); // 获取所有用户的排名及其信息
    QString add(QString n, QString p); // 注册后添加用户

private:
    QList<User>users; // 用户信息列表（不论顺序，会按照积分来排序）
    QString filePath;
};

#endif // USERLIST_H
