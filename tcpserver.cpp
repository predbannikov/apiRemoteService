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

    initKeyButton();
    insert_text("");
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
//    usleep(1000);
    if(_jobj["action"] == "move_cursor") {
        move_cursor(_jobj["x"].toInt(), _jobj["y"].toInt());
        qDebug() << "move_cursor" << _jobj["x"].toInt() << _jobj["y"].toInt();
    } else if(_jobj["action"] == "input_click") {
        if(_jobj["TYPE_BUTTON"].toString() == "BTN_LEFT")
            code = BTN_LEFT;
        else if(_jobj["TYPE_BUTTON"].toString() == "BTN_RIGHT")
            code = BTN_RIGHT;
        input_click(_jobj["x"].toInt(), _jobj["y"].toInt(), code);
        qDebug() << "input_click" << _jobj["x"].toInt() << _jobj["y"].toInt() << _jobj["TYPE_BUTTON"].toString();
    } else if(_jobj["action"] == "simple_click_up") {
        if(_jobj["TYPE_BUTTON"].toString() == "BTN_LEFT")
            code = BTN_LEFT;
        else if(_jobj["TYPE_BUTTON"].toString() == "BTN_RIGHT")
            code = BTN_RIGHT;
        simple_click_up(code);
        qDebug() << "input_click" << _jobj["TYPE_BUTTON"].toString();
    } else if(_jobj["action"] == "input_click_up") {
        if(_jobj["TYPE_BUTTON"].toString() == "BTN_LEFT")
            code = BTN_LEFT;
        else if(_jobj["TYPE_BUTTON"].toString() == "BTN_RIGHT")
            code = BTN_RIGHT;
        input_click_up(_jobj["x"].toInt(), _jobj["y"].toInt(), code);
        qDebug() << "input_click" << _jobj["x"].toInt() << _jobj["y"].toInt() << _jobj["TYPE_BUTTON"].toString();
    } else if(_jobj["action"] == "release_button") {
        if(_jobj["TYPE_BUTTON"].toString() == "BTN_LEFT")
            code = BTN_LEFT;
        else if(_jobj["TYPE_BUTTON"].toString() == "BTN_RIGHT")
            code = BTN_RIGHT;
        release_button(code);
        qDebug() << "release_button" << _jobj["TYPE_BUTTON"].toString();
    } else if(_jobj["action"] == "send_syn") {
        send_syn();
        qDebug() << "send_syn";
    } else if(_jobj["action"] == "destroy_app") {
        destroy_app();
        qDebug() << "destroy_app";
    } else if(_jobj["insert_text"] == "insert_text") {
        insert_text(_jobj["text"].toString());
        qDebug() << "insert_text" << _jobj["text"].toString();
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

void TcpServer::initKeyButton()
{

    keys.insert('0', KEY_0);
    keys.insert('1', KEY_1);
    keys.insert('2', KEY_2);
    keys.insert('3', KEY_3);
    keys.insert('4', KEY_4);
    keys.insert('5', KEY_5);
    keys.insert('6', KEY_6);
    keys.insert('7', KEY_7);
    keys.insert('8', KEY_8);
    keys.insert('9', KEY_9);
    keys.insert('Q', KEY_Q);
    keys.insert('q', KEY_Q);
    keys.insert('W', KEY_W);
    keys.insert('w', KEY_W);
    keys.insert('E', KEY_E);
    keys.insert('e', KEY_E);
    keys.insert('R', KEY_R);
    keys.insert('r', KEY_R);
    keys.insert('T', KEY_T);
    keys.insert('t', KEY_T);
    keys.insert('Y', KEY_Y);
    keys.insert('y', KEY_Y);
    keys.insert('U', KEY_U);
    keys.insert('u', KEY_U);
    keys.insert('I', KEY_I);
    keys.insert('i', KEY_I);
    keys.insert('O', KEY_O);
    keys.insert('o', KEY_O);
    keys.insert('P', KEY_P);
    keys.insert('p', KEY_P);
    keys.insert('a', KEY_A);
    keys.insert('A', KEY_A);
    keys.insert('s', KEY_S);
    keys.insert('S', KEY_S);
    keys.insert('d', KEY_D);
    keys.insert('D', KEY_D);
    keys.insert('F', KEY_F);
    keys.insert('f', KEY_F);
    keys.insert('G', KEY_G);
    keys.insert('g', KEY_G);
    keys.insert('h', KEY_H);
    keys.insert('H', KEY_H);
    keys.insert('j', KEY_J);
    keys.insert('J', KEY_J);
    keys.insert('k', KEY_K);
    keys.insert('K', KEY_K);
    keys.insert('L', KEY_L);
    keys.insert('l', KEY_L);
    keys.insert('z', KEY_Z);
    keys.insert('Z', KEY_Z);
    keys.insert('X', KEY_X);
    keys.insert('x', KEY_X);
    keys.insert('C', KEY_C);
    keys.insert('c', KEY_C);
    keys.insert('V', KEY_V);
    keys.insert('v', KEY_V);
    keys.insert('B', KEY_B);
    keys.insert('b', KEY_B);
    keys.insert('N', KEY_N);
    keys.insert('n', KEY_N);
    keys.insert('M', KEY_M);
    keys.insert('m', KEY_M);
    keys.insert(' ', KEY_SPACE);
    keys.insert('.', KEY_DOT);
    keys.insert('*', KEY_KPASTERISK);
    keys.insert('\'', KEY_APOSTROPHE);
    keys.insert(';', KEY_SEMICOLON);
    keys.insert(']', KEY_RIGHTBRACE);
    keys.insert('[', KEY_LEFTBRACE);
    keys.insert(',', KEY_COMMA);
    keys.insert('=', KEY_EQUAL);
    keys.insert('-', KEY_MINUS);
    keys.insert('+', KEY_KPPLUS);
    keys.insert('`', KEY_GRAVE);
    keys.insert('/', KEY_SLASH);
    keys.insert('\\', KEY_BACKSLASH);

}

void TcpServer::insert_text(QString t_text)
{
    t_text = "hello1234567890-=!@#$%^&*()_+qwertyuiop[]\\|}{asdfghjk,nvxz<>/?";
    for(int i = 0; i < t_text.size(); i++) {
        QChar qch = t_text[i];
        char ch = qch.toLatin1();
        switch (ch) {
        case '!':
            press_shift_key(KEY_1); break;
        case '@':
            press_shift_key(KEY_2); break;
        case '#':
            press_shift_key(KEY_3); break;
        case '$':
            press_shift_key(KEY_4); break;
        case '%':
            press_shift_key(KEY_5); break;
        case '^':
            press_shift_key(KEY_6); break;
        case '&':
            press_shift_key(KEY_7); break;
        case '(':
            press_shift_key(KEY_9); break;
        case ')':
            press_shift_key(KEY_0); break;
        case '_':
            press_shift_key(KEY_MINUS); break;
        default:
            __U16_TYPE code = keys[t_text[i]];
            press_key(code);
        }
    }
}

