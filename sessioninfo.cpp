#include "sessioninfo.h"

SessionInfo::SessionInfo(QTcpSocket *s)
{
    socket = s;
    session_id = -1;
    table_id = -1;
    userID = "";

    connect(socket, QTcpSocket::readyRead, [=]{
        char buffer[1024] = {0};
        socket->read(buffer, 1024);

        if (strlen(buffer) > 0)
        {
            QString text(buffer);
            emit signalReadData(session_id,text);
        }
        else
        {
            emit signalReadData(session_id,"接收数据错误");
        }
    });

    connect(socket, QTcpSocket::disconnected, [=]{
        emit signalDisconnect(session_id);
    });
}

int SessionInfo::getSessionId()
{
    return session_id;
}

void SessionInfo::setInfo(QString userID, QString username, QString password)
{
    this->userID = userID;
    this->username = username;
    this->password = password;
}

void SessionInfo::setSessionId(int id)
{
    this->session_id = id;
}

void SessionInfo::setInfo(int table_id, int table_seat)
{
    this->table_id = table_id;
    this->table_seat = table_seat;
}

int SessionInfo::getTableId()
{
    return table_id;
}

int SessionInfo::getTableSeat()
{
    return table_seat;
}

bool SessionInfo::response(QString data)
{
    char chars[1024] = {0};
    strcpy_s(chars, data.toStdString().c_str());
    if (!socket->isValid()) return false;
    int rst = socket->write(chars, strlen(chars));
    if (rst == -1)
        return false;
    return true;
}

QString SessionInfo::getUserID()
{
    return userID;
}

QString SessionInfo::getUsername()
{
    return username;
}

bool SessionInfo::isReady()
{
    return ready;
}

QTcpSocket *SessionInfo::getSocket()
{
    return socket;
}
