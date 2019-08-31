#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QCoreApplication>
#include <QObject>
#include <QDebug>
#include <QTcpServer>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QThread>
#include <QJsonObject>
#include <QJsonDocument>

class TcpServer : public QObject
{
    Q_OBJECT
    QTcpServer *tcpServer = nullptr;

    void slotSendMessage(QTcpSocket *socket, QString t_message);
    void initKeyButton();
    void insert_text(QString t_text);
    void push_sequence_button(QString t_buttons);
    QMap <QChar, __U16_TYPE> keys;
    int m_counter = 0;
public:
    explicit TcpServer(QObject *parent = nullptr);
    ~TcpServer();

signals:

private slots:
    void slotReadyRead();
    void slotNewConnection();
};

#endif // TCPSERVER_H
