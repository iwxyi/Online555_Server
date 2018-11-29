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

    void setSessionId(int id);
    int getSessionId();
    void setInfo(QString userID, QString username, QString password);
    void setInfo(int table_id, int table_seat);
    int getTableId();
    int getTableSeat();
    bool response(QString data);
    QString getUserID();
    QString getUsername();
    bool isReady();
    QTcpSocket* getSocket();

signals:
    void signalReadData(int id, QString data);
    void signalDisconnect(int id);

private:
    QTcpSocket* socket;
    int session_id;
    int table_id, table_seat;
    QString userID, username, password;
    bool ready;
};

#endif // USERINFOS_H
