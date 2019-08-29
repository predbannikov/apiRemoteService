#include "tcpserver.h"
#include "main.h"

TcpServer::TcpServer(QObject *parent) : QObject(parent)
{
    tcpServer = new QTcpServer(this);
    connect(tcpServer, &QTcpServer::newConnection, this, &TcpServer::slotNewConnection);

    if (!tcpServer->listen(QHostAddress::AnyIPv4, 10101)) {
        qDebug() << qPrintable(tr("Unable to start the server: %1.")
                              .arg(tcpServer->errorString()));
        return;
    }
    qDebug() << qPrintable(tr("The server is running on\n\nIP: %1\nport: %2\n\n")
                .arg(tcpServer->serverAddress().toString()).arg(tcpServer->serverPort()));

}

TcpServer::~TcpServer()
{
    destroy_app();
}


void TcpServer::slotReadyRead()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(QObject::sender());
    QByteArray _array = socket->readAll();
//    QString msg = _array;
//    qDebug() << qPrintable(_array);
    if(_array == "exit")
        this->deleteLater();
    QJsonObject _jobj = QJsonDocument::fromJson(_array).object();
    __U16_TYPE code = BTN_LEFT;
    if(_jobj["action"] == "move_cursor") {
        move_cursor(_jobj["x"].toInt(), _jobj["y"].toInt());
    } else if(_jobj["action"] == "input_click") {
        if(_jobj["TYPE_BUTTON"].toString() == "BTN_LEFT")
            code = BTN_LEFT;
        else if(_jobj["TYPE_BUTTON"].toString() == "BTN_RIGHT")
            code = BTN_RIGHT;
        input_click(_jobj["x"].toInt(), _jobj["y"].toInt(), code);
    } else if(_jobj["action"] == "release_button") {
        if(_jobj["TYPE_BUTTON"].toString() == "BTN_LEFT")
            code = BTN_LEFT;
        else if(_jobj["TYPE_BUTTON"].toString() == "BTN_RIGHT")
            code = BTN_RIGHT;
        release_button(code);
    } else if(_jobj["action"] == "send_syn") {
        send_syn();
    } else if(_jobj["action"] == "destroy_app") {
        destroy_app();
    }

}

void TcpServer::slotNewConnection()
{
    QTcpSocket *clientConnection = tcpServer->nextPendingConnection();
    connect(clientConnection, &QAbstractSocket::disconnected, clientConnection, &QObject::deleteLater);
    connect(clientConnection, &QAbstractSocket::readyRead, this, &TcpServer::slotReadyRead);
    QByteArray _array = QByteArray::number(0x55FF, 16);
    qDebug() << clientConnection->peerName() << clientConnection->peerAddress() << clientConnection->peerPort();
    clientConnection->write(_array);
}

void TcpServer::slotSendMessage(QTcpSocket *socket, QString t_message)
{
    socket->write(t_message.toUtf8());
}
