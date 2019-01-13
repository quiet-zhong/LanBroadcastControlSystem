#ifndef WIDGET_H
#define WIDGET_H

#include <QTimer>
#include <QWidget>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QTcpServer>
#include <QUdpSocket>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

public slots:


private slots:
    void TcpGetNewConnect();
    void TcpRecvData();
    void slotBroadcastSend();

    void slotReboot();
    void slotRestart();
    void timerSingleClick();

private:
    Ui::Widget *ui;
    QUdpSocket *m_udpSocket;
    QTcpSocket *m_tcpsocket;
    QTcpServer *m_tcpserver;

    QTimer* m_timerDoubleClicked; //判断单双击的定时器
    int isDoubleClicked; //区分是单击还是双击
};

#endif // WIDGET_H
