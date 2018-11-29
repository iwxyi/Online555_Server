#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->recorderTextEdit->document()->setMaximumBlockCount(100);

    for (int i = 0; i < 1000; i++)
        table_states[i][0] = table_states[i][1] = table_states[i][2] = 0;

    server = new QTcpServer();

    if (!server->listen(QHostAddress::Any, SERVER_PORT))
    {
        QMessageBox::information(this, "QT网络通信", "服务器端监听失败！");
        return ;
    }
    log("监听端口成功");

    log(QString("读取用户数量：%1").arg(u.count()));

    connect(server, SIGNAL(newConnection()), this, SLOT(slotNewConnection()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * 新的连接，设置socket的sessionID，并将其socket加入到集合中
 */
void MainWindow::slotNewConnection()
{
    log("\n新连接");
    QTcpSocket* socket = server->nextPendingConnection();
    if (!socket)
    {
        log("未能正确读取客户端连接");
        return ;
    }
    SessionInfo* info = new SessionInfo(socket);
    info->setSessionId(getNewSessionId());
    log(QString("设置 session_id：%1").arg(info->getSessionId()));
    sendRefresh(info);

    connect(info, SIGNAL(signalReadData(int,QString)), this, SLOT(slotReadData(int,QString)));
    connect(info, SIGNAL(signalDisconnect(int)), this, SLOT(slotDisconnect(int)));

    infos.append(info);
    log(QString("当前用户数量：%1").arg(infos.size()));
}

/**
 * 收到数据事件
 * 转发给处理函数
 * @param id   Session ID
 * @param data 数据
 */
void MainWindow::slotReadData(int id, QString data)
{
    log(QString("\n收到数据：%1：%2").arg(id).arg(data));
    slotSwitchRecv(id, data);
}

/**
 * 失去连接函数
 * 将其从socket集合中去掉
 * @param id Session ID
 */
void MainWindow::slotDisconnect(int id)
{
    log(QString("\n结束连接：%1").arg(id));
    for (int i = 0; i < infos.size(); i++)
        if (infos.at(i)->getSessionId() == id)
        {
            infos.removeAt(i);
            break;
        }
    log(QString("当前用户数量：%1").arg(infos.size()));
}

/**
 * 判断收到的数据
 * @param id  Session ID
 * @param str 收到的数据
 */
void MainWindow::slotSwitchRecv(int id, QString str)
{
    SessionInfo* si = getInfo(id);
    if (si == NULL)
    {
        log(QString("找不到 session_id:%1").arg(id));
        return;
    }
    QString kind = getXml(str, "KIND"); // 数据传过来的种类

    if (kind == "refresh") // 刷新游戏桌列表（所有玩家都刷新）
    {
        sendRefresh();
    }
    else if (kind == "register") // 注册事件
    {
        QString username = getXml(str, "USERNAME");
        QString password = getXml(str, "PASSWORD");
        log(QString("注册：username:%1, password:%2").arg(username).arg(password));
        if (!u.canRigister(username))
        {
            response(id, "<KIND>register</KIND><RESULT>0</RESULT><REASON>Username already exist</REASON>");
            return ;
        }
        QString userID = u.add(username, password);
        if (userID != "")
        {
            response(id, "<KIND>register</KIND><RESULT>1</RESULT><USERID>"+userID+"<USERID>");
            si->setInfo(userID, username, password);
        }
        else
        {
            response(id, "<KIND>register</KIND><RESULT>0</RESULT>");
        }
    }
    else if (kind == "login") // 登录事件
    {
        QString username = getXml(str, "USERNAME");
        QString password = getXml(str, "PASSWORD");
        log(QString("登录：username:%1, password:%2").arg(username).arg(password));
        QString userID = u.exists(username, password);
        if (userID != "")
        {
            if (isLogining(username))
            {
                response(id, "<KIND>login</KIND><RESULT>0</RESULT><REASON>Account has been logined in other device.</REASON>");
            }
            else
            {
                int integral = u.getIntegral(userID);
                int rank = u.getRank(userID);
                QString res = makeXml("login", "KIND") + makeXml(1, "RESULT") + makeXml(userID, "USERID");
                res += makeXml(integral, "INTEGRAL") + makeXml(username, "USERNAME") + makeXml(rank, "RANK");
                //response(id, "<KIND>login</KIND><RESULT>1</RESULT><USERID>"+userID+"</USERID><INTEGRAL>"+integral+"</INTEGRAL>");
                response(id, res);
                si->setInfo(userID, username, password);
            }
        }
        else
        {
            response(id, "<KIND>login</KIND><RESULT>0</RESULT>");
        }
    }
    else if (kind == "enter") // 加入游戏桌（1/2）
    {
        int table_id = getXml(str, "TABLE_ID").toInt();
        int table_seat = getXml(str, "TABLE_SEAT").toInt();
        if (table_id <= 0 || table_id > TABLE_ALL) return ;
        if (table_seat <= 0 || table_seat > 2) return ;
        if (table_states[table_id][table_seat] > 0)
        {
            QString res = makeXml("error", "KIND")+makeXml(0, "RESULT")+makeXml("No seat", "REASON");
            response(id, res);
        }
        log(QString("加入游戏桌：%1, %2").arg(table_id).arg(table_seat));
        table_states[table_id][table_seat] = 1;
        si->setInfo(table_id, table_seat);

        int reverse = table_seat == 1 ? 2 : 1;
        if (table_states[table_id][reverse] > 0) // 有对手
        {
            if (table_states[table_id][reverse] == 2) // 对手已准备好
            {
                QString res = makeXml("ready", "KIND");
                response(si, res);
            }

            SessionInfo* info = getInfo(table_id, reverse); // 对手
            if (info != NULL)
            {
                QString res = makeXml("come", "KIND");
                response(info, res);
            }
        }
        else
        {
            log("暂时没有对手");
        }

        sendRefresh(); // 针对所有用户刷新桌面
    }
    else if (kind == "leave") // 离开游戏桌。如果正在对战，则中止
    {
        int table_id = si->getTableId();
        int table_seat = si->getTableSeat();
        if (table_id == -1)
            table_id = getXml(str, "TABLE_ID").toInt();
        if (table_seat == -1)
            table_seat = getXml(str, "TABLE_SEAT").toInt();
        if (table_id <= 0 || table_id > TABLE_ALL) return ;
        if (table_seat <= 0 || table_seat > 2) return ;
        if (table_id > 0 && table_seat > 0 && table_states[table_id][table_seat] == 0)
        {
            QString res = makeXml("error", "KIND")+makeXml(0, "RESULT")+makeXml("No person", "REASON");
            response(id, res);
            return ;
        }

        int reverse = table_seat == 1 ? 2 : 1;
        if ( table_states[table_id][table_seat] == 2 && table_states[table_id][reverse] == 2) // 正在对战
        {
            SessionInfo* info = getInfo(table_id, reverse); // 对手
            if (info != NULL)
            {
                QString res = makeXml("alone", "KIND");
                response(info, res);
            }
        }
        log(QString("离开座位：%1,%2").arg(table_id).arg(table_seat));
        table_states[table_id][table_seat] = 0; // 离开清零
        si->setInfo(0, 0);

        sendRefresh(); // 针对所有用户刷新桌面
    }
    else if (kind == "ready") // 一方准备好。如果另一方也准备了，那么立即开始对战
    {
        int table_id = si->getTableId();
        int table_seat = si->getTableSeat();
        if (table_id == -1)
            table_id = getXml(str, "TABLE_ID").toInt();
        if (table_seat == -1)
            table_seat = getXml(str, "TABLE_SEAT").toInt();
        if (table_id <= 0 || table_id > TABLE_ALL) return ;
        if (table_seat <= 0 || table_seat > 2) return ;
        if (table_id > 0 && table_seat > 0 && table_states[table_id][table_seat] == 0)
        {
            QString res = makeXml("error", "KIND")+makeXml(0, "RESULT")+makeXml("No person", "REASON");
            response(id, res);
            return ;
        }

        table_states[table_id][table_seat] = 2; // 准备 = 2

        int reverse = table_seat == 1 ? 2 : 1;
        SessionInfo* info = getInfo(table_id, reverse);

        if (info != NULL) // 对方不是 NULL（调试时只有一方，所以需要手动 start）
        {
            QString res = makeXml("ready", "KIND"); // 这个ready是指对方的
            response(info, res);

            if (table_states[table_id][reverse] == 2) // 另一方也准备好了，立即开始对战
            {
                QString res2 = makeXml("start", "KIND");
                response(si, res2); // 自己开始
                info->getSocket()->waitForBytesWritten(100); // ☆☆☆ 多次response一定要加这句话，不然会在缓冲区连到一起
                                                             // 最后变成 <KIND>ready</KIND> <KIND>start</KIND>
                response(info, res2); // 对方开始
            }
            else
            {
                log(QString("等待对方的 ready，位于%1，%2").arg(table_id).arg(reverse));
            }
        }
        else
        {
            log(QString("没有能发送落子对象的 session，位于%1，%2").arg(table_id).arg(reverse));
        }
    }
    else if (kind == "notready") // 取消准备（只有唯一准备的一方才可以）
    {
        int table_id = si->getTableId();
        int table_seat = si->getTableSeat();
        if (table_id == -1)
            table_id = getXml(str, "TABLE_ID").toInt();
        if (table_seat == -1)
            table_seat = getXml(str, "TABLE_SEAT").toInt();
        if (table_id <= 0 || table_id > TABLE_ALL) return ;
        if (table_seat <= 0 || table_seat > 2) return ;
        if (table_id > 0 && table_seat > 0 && table_states[table_id][table_seat] == 0)
        {
            QString res = makeXml("error", "KIND")+makeXml(0, "RESULT")+makeXml("No person", "REASON");
            response(id, res);
        }

        table_states[table_id][table_seat] = 1; // 未准备 = 1

        int reverse = table_seat == 1 ? 2 : 1;
        SessionInfo* info = getInfo(table_id, reverse);

        if (info != NULL)
        {
            QString res = makeXml("notready", "KIND"); // 这个notready也是指对方的
            response(info, res);
        }
    }
    else if (kind == "start") ; // 开始游戏，这个是发过去的
    else if (kind == "moves") // 落子
    {
        int table_id = si->getTableId();
        int table_seat = si->getTableSeat();
        if (table_id == -1)
            table_id = getXml(str, "TABLE_ID").toInt();
        if (table_seat == -1)
            table_seat = getXml(str, "TABLE_SEAT").toInt();
        if (table_id < 0 || table_id > TABLE_ALL) return ;
        if (table_seat < 0 || table_seat > 2) return ;
        if (table_id > 0 && table_seat > 0 && table_states[table_id][table_seat] == 0)
        {
            QString res = makeXml("error", "KIND")+makeXml(0, "RESULT")+makeXml("No person", "REASON");
            response(id, res);
        }

        int reverse = table_seat == 1 ? 2 : 1;
        SessionInfo* info = getInfo(table_id, reverse);

        if (info != NULL)
        {
            response(info, str); // 直接原封不动的发过去……这个……好像并没有什么问题
        }
        else
        {
            log(QString("没有能发送落子对象的 session，位于%1，%2").arg(table_id).arg(reverse));
        }
    }
    else if (kind == "over") // 游戏结束，赢家增加积分
    {
        int table_id = si->getTableId();
        int table_seat = si->getTableSeat();
        if (table_id == -1)
            table_id = getXml(str, "TABLE_ID").toInt();
        if (table_seat == -1)
            table_seat = getXml(str, "TABLE_SEAT").toInt();
        if (table_id < 0 || table_id > TABLE_ALL) return ;
        if (table_seat < 0 || table_seat > 2) return ;
        if (table_id > 0 && table_seat > 0 && table_states[table_id][table_seat] == 0)
        {
            QString res = makeXml("error", "KIND")+makeXml(0, "RESULT")+makeXml("No person", "REASON");
            response(id, res);
        }

        int winner = getXml(str, "WINNER").toInt();
        int nums = getXml(str, "PIECE_NUM").toInt();
        if (nums <= 3 || nums > 512) // 最大规则是32*32，减半
        {
            QString res = makeXml("error", "KIND")+makeXml(0, "RESULT")+makeXml("Piece nums error", "REASON");
            response(id, res);
            return ;
        }

        int igr_winner = 1000-nums;
        int igr_failer = 300+nums;
        u.addIntegral(si->getUserID(), igr_winner); // 增加奖励

        int reverse = table_seat == 1 ? 2 : 1;
        SessionInfo* info = getInfo(table_id, reverse);
        if (info != NULL)
        {
            u.addIntegral(info->getUserID(), igr_failer); // 增加奖励
        }

        QString res_winner = makeXml("over", "KIND") + makeXml(winner, "WINNER") + makeXml(igr_winner, "REWARD");
        res_winner += makeXml(u.getIntegral(si->getUserID()), "INTEGRAL") + makeXml(u.getRank(si->getUserID()), "RANK");
        response(si, res_winner);

        if (info != NULL)
        {
            QString res_failer = makeXml("over", "KIND") + makeXml(winner, "WINNER") + makeXml(igr_failer, "REWARD");
            res_failer += makeXml(u.getIntegral(info->getUserID()), "INTEGRAL") + makeXml(u.getRank(info->getUserID()), "RANK");
            response(info, res_failer);
        }
        else
        {
            log(QString("没有能发送落子对象的 session，位于%1，%2").arg(table_id).arg(reverse));
        }

        table_states[table_id][1] = table_states[table_id][2] = 1;
    }
}

/**
 * 向所有客户端发送游戏桌人数数据
 */
void MainWindow::sendRefresh()
{
    QString res = getAllTableStates();

    for (SessionInfo* info : infos) // 每个用户都刷新一遍
    {
        response(info, res);
    }
}

/**
 * 向单个客户端发送游戏桌人数数据
 * 一般用于某个用户的注册、登录
 * @param si 客户端连接对象
 */
void MainWindow::sendRefresh(SessionInfo *si)
{
    response(si, getAllTableStates());
}

/**
 * 获取所有游戏桌的人数数据
 * 用来发送给客户端
 * @return 游戏桌人数数据
 */
QString MainWindow::getAllTableStates()
{
    QString res = makeXml("refresh", "KIND");

    // <TABLE><USERNAME1></USERNAME1><STATE1></STATE1></TABLE>
    res += "<TABLES>";
    for (int i = 1; i <= TABLE_ALL; i++)
    {
        res += "<T>"; // TABLE
        {
            SessionInfo* si = getInfo(i, 1);
            if (si != NULL)
            {
                res += makeXml(si->getUsername(), "USERNAME1");
                if (si->isReady())
                    res += makeXml(2, "STATE1");
                else
                    res += makeXml(1, "STATE1");
            }
        }
        {
            SessionInfo* si = getInfo(i, 2);
            if (si != NULL)
            {
                res += makeXml(si->getUsername(), "USERNAME2");
                if (si->isReady())
                    res += makeXml(2, "STATE2");
                else
                    res += makeXml(1, "STATE2");
            }
        }
        res += "</T>";
    }
    res += "</TABLES>";

    return res;
}

/**
 * 为每一个连接创建一个ID，用来区分客户端
 * 如果随机ID重复了，递归随机产生
 * @return ID
 */
int MainWindow::getNewSessionId()
{
    int id = rand() % 900000+100000; // 六位数
    for (SessionInfo* info : infos)
    {
        int tid = info->getSessionId();
        if (tid == id) // 重复的 session_id
            return getNewSessionId();
    }
    return id;
}

/**
 * 根据ID找到连接对象
 * @param  session_id 连接 ID
 * @return            连接的对象
 */
SessionInfo *MainWindow::getInfo(int session_id)
{
    for (SessionInfo* info : infos)
        if (info->getSessionId() == session_id)
            return info;
    return NULL;
}

/**
 * 根据玩家位置找到连接对象
 * @param  table_id   游戏桌ID
 * @param  table_seat 游戏桌座位位置
 * @return            连接对象
 */
SessionInfo* MainWindow::getInfo(int table_id, int table_seat)
{
    for (SessionInfo* info : infos)
        if (info->getTableId() == table_id && info->getTableSeat() == table_seat)
            return info;
    return NULL;
}

/**
 * 显示数据在文本框中
 * @param str 显示的数据
 */
void MainWindow::log(QString str)
{
    ui->recorderTextEdit->append(str);
    QScrollBar* scrollbar = ui->recorderTextEdit->verticalScrollBar();
    scrollbar->setSliderPosition(scrollbar->maximum());
}

/**
 * 是否已登录（不允许多个设备登录来刷积分）
 * @param username 用户名
 * @return 是否已在其他设备登录
 */
bool MainWindow::isLogining(QString username)
{
    for (SessionInfo* si : infos)
        if (si->getUsername() == username)
            return true;
    return false;
}

/**
 * 发送/回复数据给某个客户端
 * @param id   连接ID
 * @param data 数据
 */
void MainWindow::response(int id, QString data)
{
    SessionInfo* si = getInfo(id);
    if (si->response(data))
        log(QString("回复数据：%1 - %2").arg(id).arg(data));
    else
        log(QString("回复数据失败：%1 - %2").arg(id).arg(data));
}

/**
 * 发送/回复数据给某个客户端
 * @param si   连接对象
 * @param data 数据
 */
void MainWindow::response(SessionInfo* si, QString data)
{
    if (si->response(data))
        log(QString("回复数据：%1 - %2").arg(si->getSessionId()).arg(data));
    else
        log(QString("回复数据失败：%1 - %2").arg(si->getSessionId()).arg(data));
}

/**
 * 伪造数据发送给特定的客户端
 * 以便于测试（不然没法调试啊）
 */
void MainWindow::on_pushButton_clicked()
{
    int id = ui->lineEdit->text().toInt();
    int seat = ui->lineEdit_2->text().toInt();
    QString data = ui->lineEdit_3->text();
    if (id == 0 || seat == 0 || data == "") return ;
    SessionInfo* si = getInfo(id, seat);
    if (seat != NULL)
        response(si, data);
    //ui->lineEdit_3->setText("");
}
