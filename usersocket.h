#ifndef SOCKET_H
#define SOCKET_H

#include <QObject>
#include <QTcpSocket>
#include <QString>
#include <QMessageBox>

class UserSocket : public QTcpSocket
{
    Q_OBJECT
public:
    UserSocket(QTcpSocket* s);

    void setInfo(QString userID, int table_id, int table_seat);

signals:
    void signalReadyRead(int index);
    void signalDisConnection(int index);

public slots:
    void slotReadyRead();
    void slotDisConnection();

private:
    int table_id, table_seat; // 座位位置
    QString userID;
    QString username, password;
    bool ready;
};

#endif // SOCKET_H
