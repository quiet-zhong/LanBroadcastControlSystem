#include "widget.h"
#include "ui_widget.h"

#include <QString>
#include <QByteArray>
#include <QDebug>
#include <QThread>


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    isDoubleClicked = 0; //初始化不点击
    m_timerDoubleClicked = new QTimer(this); //新建双击定时器
    connect(m_timerDoubleClicked,SIGNAL(timeout()),this,SLOT(timerSingleClick()));

    connect(ui->lineEdit,SIGNAL(editingFinished()),ui->pushButton,SLOT(click()));
    connect(ui->pushButton,SIGNAL(pressed()),this,SLOT(slotBroadcastSend()));

    connect(ui->pushButton_reboot,SIGNAL(pressed()),this,SLOT(slotReboot()));
    connect(ui->pushButton_restart,SIGNAL(pressed()),this,SLOT(slotRestart()));

    m_udpSocket = new QUdpSocket(this);
    m_tcpserver = new QTcpServer(this);
    connect(m_tcpserver,SIGNAL(newConnection()),this,SLOT(TcpGetNewConnect()));
    if(!m_tcpserver->listen(QHostAddress::Any, 54546))
    {
        //若出错，则输出错误信息
        qDebug() << m_tcpserver->errorString();
        return;
    }
}

Widget::~Widget()
{
    delete m_udpSocket;
    delete m_tcpserver;
    delete m_timerDoubleClicked;
    delete ui;
}

void Widget::TcpGetNewConnect()
{
    //获取客户端连接
    m_tcpsocket = m_tcpserver->nextPendingConnection();
    connect(m_tcpsocket, SIGNAL(readyRead()), this, SLOT(TcpRecvData()));

    //    qDebug() << "A Client connect!";
}

void Widget::TcpRecvData()
{
    QByteArray buffer;
    //读取缓冲区数据
    buffer = m_tcpsocket->readAll();
    if(!buffer.isEmpty())
    {
        QString strRecv = QString(buffer).mid(1,buffer.length()-3);
        //        qDebug() << strRecv;

        QString strPre = strRecv.section('.',0,0);
        bool ok = false;
        int ret = strPre.toInt(&ok);
        //        qDebug() << strPre << ret;
        if(ret == 0)
            ui->textBrowser->append("<font color = green>========== 成功 ==========</font>");
        else
            ui->textBrowser->append("<font color = red>========== 失败 ==========</font>");

        QString strEnd = strRecv.mid(strPre.length()+3, strRecv.length()-strPre.length()-6);
        strEnd.replace("\\n","\n");
        qDebug() << strEnd;
        ui->textBrowser->append(strEnd);
    }
}

void Widget::slotBroadcastSend()
{
    QThread::usleep(1000);
    if(ui->lineEdit->text() != "")
    {
        qDebug() << ui->lineEdit->text();
        ui->textBrowser->clear();

        m_udpSocket->writeDatagram(ui->lineEdit->text().toLatin1().data(), ui->lineEdit->text().length(),
                                   QHostAddress::Broadcast,
                                   54545);

        ui->lineEdit->clear();
        ui->textBrowser->setText("<font color = blue> 执行成功 </font>");
    }
    else
    {
        qDebug() << "no input";
    }
}

void Widget::slotReboot()
{
    isDoubleClicked++;
    if(isDoubleClicked == 1)
        m_timerDoubleClicked->start(400); //100ms为自己设置
    if(isDoubleClicked == 2)
    {
        isDoubleClicked = 0;
        m_timerDoubleClicked->stop();
        //按钮双击响应代码放这里
        qDebug() << "双击响应";
        ui->lineEdit->setText("reboot");
        qDebug() << "reboot";
        slotBroadcastSend();
        ui->textBrowser->setText( "<font color = blue> 重启 </font>");
    }
}

void Widget::timerSingleClick()
{
    isDoubleClicked = 0;
    m_timerDoubleClicked->stop();
    //按钮单击响应代码放这里
    qDebug() << "单击响应";
    ui->textBrowser->setText( "<font color = blue> 单击无效 </font>");
}


void Widget::slotRestart()
{
    ui->lineEdit->setText("/update/service.sh &");
    slotBroadcastSend();
    qDebug() << "restart";
}
