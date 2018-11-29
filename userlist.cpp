#include "userlist.h"

UserList::UserList()
{
    filePath = DataPath + "users.dat";
    ensureDirExist(DataPath);
    ensureFileExist(filePath);
    readFromFile();
}

int UserList::count()
{
    return users.size();
}

int UserList::readFromFile()
{
    users.clear();
    if (!isFileExist(filePath)) return -1;
    QString text = readTextFile(filePath);
    QStringList list = getStrMids(text, "<USER>", "</USER>");

    for (QString s : list)
    {
        QString userID = getXml(s, "USERID");
        QString username = getXml(s, "USERNAME");
        QString password = getXml(s, "PASSWORD");
        QString s_integral = getXml(s, "INTEGRAL");
        int integral = s_integral.toInt();
        User user(userID, username, password, integral);
        users.append(user);
    }
    return users.size();
}

bool UserList::writeToFile()
{
    QString text = "";
    for (int i = 0; i < users.size(); i++)
    {
        User user = users.at(i);
        QString userID = makeXml(user.userID, "USERID");
        QString username = makeXml(user.username, "USERNAME");
        QString password = makeXml(user.password, "PASSWORD");
        QString integral = makeXml(QString("%1").arg(user.integral), "INTEGRAL");
        text += QString("<USER>%1%2%3%4</USER>").arg(userID).arg(username).arg(password).arg(integral);
    }
    return writeTextFile(filePath, text);
}

void UserList::addIntegral(QString userID, int addition)
{
    for (User& user : users)
    {
        if (user.userID == userID)
        {
            user.integral += addition;
            break;
        }
    }
    writeToFile(); // 保存修改
}

bool UserList::canRigister(QString n)
{
    for (User user : users)
    {
        if (user.username == n)
            return false;
    }
    return true;
}

QString UserList::exists(QString n, QString p)
{
    for (User user : users)
    {
        if (user.username == n && user.password == p)
            return user.userID;
    }
    return "";
}

QString UserList::add(QString n, QString p)
{
    if (exists(n, p) != "")
        return "";

    User user(getRandomUserID(), n, p, 0);
    users.append(user);

    writeToFile();

    return user.userID;
}

QString UserList::getRandomUserID()
{
    int id = rand() % 900000000 + 1000000000;
    QString ids = QString("%1").arg(id);

    for (User user : users)
        if (user.userID == ids) // 如果有相同的，则递归
            return getRandomUserID();

    return ids;
}

int UserList::getIntegral(QString userID)
{
    for (User user : users)
    {
        if (user.userID == userID)
            return user.integral;
    }
    return 0;
}

int cmp(User a, User b)
{
    return a.integral > b.integral;
}

int UserList::getRank(QString userID)
{
    int integral = getIntegral(userID);

    qSort(users.begin(), users.end(), cmp);

    for (int i = 0; i < users.size(); i++)
        if (users.at(i).integral <= integral)
        {
            return i+1;
        }
    return users.size();
}
