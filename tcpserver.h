#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QCoreApplication>
#include <QObject>
#include <QDebug>
#include <QTcpServer>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QJsonObject>
#include <QJsonDocument>

class TcpServer : public QObject
{
    Q_OBJECT
    QTcpServer *tcpServer = nullptr;

    void slotSendMessage(QTcpSocket *socket, QString t_message);
public:
    explicit TcpServer(QObject *parent = nullptr);
    ~TcpServer();

signals:

private slots:
    void slotReadyRead();
    void slotNewConnection();
};

#endif // TCPSERVER_H
