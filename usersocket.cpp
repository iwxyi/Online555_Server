#include "usersocket.h"

UserSocket::UserSocket(QTcpSocket *s)
{
    table_id = -1;
    userID = "";

    connect(this, SLOT(readyRead()), this, SLOT(slotReadyRead()));
    connect(this, SLOT(disconnect()), this, SLOT(slotDisConnection()));

    connect(this, QTcpSocket::readyRead, [=]{
        QMessageBox::information(NULL, "dwefvf", "def");
    });
}

void UserSocket::setInfo(QString userID, int tableID, int table_seat)
{
    this->table_id = tableID;
    this->userID = userID;
    this->table_seat = table_seat;
}

void UserSocket::slotReadyRead()
{
    emit signalReadyRead(table_id);
}

void UserSocket::slotDisConnection()
{
    emit signalDisConnection(table_id);
}
