#ifndef USERINFOS_H
#define USERINFOS_H

#include <QObject>
#include <QTcpSocket>
#include <QMessageBox>
#include "globals.h"

class SessionInfo : public QObject
{
    Q_OBJECT
public:
    SessionInfo(QTcpSocket* s);

    void setSessionId(int id);   // 设置ID
    int getSessionId();          // 获取ID，判断身份
    void setInfo(QString userID, QString username, QString password);
    void setInfo(int table_id, int table_seat);
    int getTableId();            // 游戏桌ID
    int getTableSeat();          // 游戏桌座位
    bool response(QString data); // 回复数据给该客户端
    QString getUserID();         // 获取用户ID
    QString getUsername();       // 用户用户昵称
    bool isReady();              // 是否准备完毕
    QTcpSocket* getSocket();     // 获取套接字对象

signals:
    void signalReadData(int id, QString data); // 收到数据的信号
    void signalDisconnect(int id); // 失去连接的信号

private:
    QTcpSocket* socket;
    int session_id; // 唯一会话ID
    int table_id, table_seat; // 游戏桌以及位置
    QString userID, username, password; // 用户信息
    bool ready; // 准备状态
};

#endif // USERINFOS_H
