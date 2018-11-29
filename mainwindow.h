#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QTcpServer>
#include <QList>
#include <QScrollBar>
#include "sessioninfo.h"
#include "globals.h"
#include "userlist.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void slotNewConnection();
    void slotReadData(int id, QString data);
    void slotDisconnect(int id);
    void slotSwitchRecv(int id, QString str);

private slots:
    void on_pushButton_clicked();

private:
    int getNewSessionId();
    void response(int id, QString data);
    void response(SessionInfo* si, QString data);
    void sendRefresh();
    void sendRefresh(SessionInfo* si);
    QString getAllTableStates();
    SessionInfo* getInfo(int session_id);
    SessionInfo* getInfo(int table_id, int table_seat);
    void log(QString str);
    bool isLogining(QString username);

private:
    Ui::MainWindow *ui;
    QTcpServer* server;
    UserList u;
    QList<SessionInfo*>infos;
    int table_states[1000][3]; // 记录游戏桌是否有人的状态
};

#endif // MAINWINDOW_H
