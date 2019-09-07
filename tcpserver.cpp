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
    initFKeyButton();
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
    QJsonObject _jObjResponse;
    if(_array == "exit")
        this->deleteLater();
    QJsonObject _jObj = QJsonDocument::fromJson(_array).object();
    __U16_TYPE code = BTN_LEFT;

    if(_jObj["target"].toString() == "keyboard") {                          // ******** KEYBOARD *********
        if(_jObj["method"].toString() == "type") {                          // type
            push_sequence_button(_jObj["text"].toString());
        } else if(_jObj["method"].toString() == "push_f") {                 // push functional key
            push_F_button(_jObj["f_code"].toString());
        } else if(_jObj["method"].toString() == "press_f") {                // press functional key
            press_F_button(_jObj["f_code"].toString());
        } else if(_jObj["method"].toString() == "release_f") {              // release functional key
            release_F_button(_jObj["f_code"].toString());
        } else if(_jObj["method"].toString() == "press") {                  // press
            QChar _qch = _jObj["code"].toString()[0];
            press_button_key(keys[_qch]);
        } else if(_jObj["method"].toString() == "release") {                // release
            QChar _qch = _jObj["code"].toString()[0];
            release_button_key(keys[_qch]);
        }
    } else if(_jObj["target"].toString() == "mouse") {                      // ******** MOUSE *********
        if(_jObj["method"] == "move") {                                     // move
            mouse_move(_jObj["x"].toInt(), _jObj["y"].toInt());
        } else if(_jObj["method"].toString() == "move_click") {             // move and click
            code =_jObj["code"].toString() == "BTN_LEFT" ? BTN_LEFT : BTN_RIGHT;
            mouse_move_click(_jObj["x"].toInt(), _jObj["y"].toInt(), code);
        } else if(_jObj["method"].toString() == "click") {                  // click
            code =_jObj["code"].toString() == "BTN_LEFT" ? BTN_LEFT : BTN_RIGHT;
            mouse_click(code);
        } else if(_jObj["method"].toString() == "move_press") {             // move and press
            code =_jObj["code"].toString() == "BTN_LEFT" ? BTN_LEFT : BTN_RIGHT;
            mouse_move_press(_jObj["x"].toInt(), _jObj["y"].toInt(), code);
        } else if(_jObj["method"].toString() == "move_release") {           // move and release
            code =_jObj["code"].toString() == "BTN_LEFT" ? BTN_LEFT : BTN_RIGHT;
            mouse_move_release(_jObj["x"].toInt(), _jObj["y"].toInt(), code);
        }
    } else if(_jObj["target"].toString() == "server") {                     // ******** SERVER *********
        if(_jObj["method"].toString() == "destroy") {                       // destroy
            destroy_app();
        }
    } else {
        _jObjResponse["code"] = "error";
        socket->write(QJsonDocument(_jObjResponse).toJson());
        return;
    }
    QString _strOut;
    QStringList _keys = _jObj.keys();
    for(QString _key: _keys) {
        if(_key == "target" || _key == "method" || _key == "code" || _key == "text") {
            _strOut.append(_key + ":" +  _jObj[_key].toString() + "\n");
        } else if(_key == "x" || _key == "y") {
            _strOut.append(_key + ":" + QString::number(_jObj[_key].toInt()) + "\n");
        }
    }
    qDebug() << qPrintable(_strOut);
    qDebug() << "counter =" << m_counter << "\n\n";
    _jObjResponse["code"] = "ok";
    socket->write(QJsonDocument(_jObjResponse).toJson());
    m_counter++;
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

void TcpServer::initFKeyButton()
{
    m_Fkeys.insert("KEY_F1", KEY_F1);
    m_Fkeys.insert("KEY_F2", KEY_F2);
    m_Fkeys.insert("KEY_F3", KEY_F3);
    m_Fkeys.insert("KEY_F4", KEY_F4);
    m_Fkeys.insert("KEY_F5", KEY_F5);
    m_Fkeys.insert("KEY_F6", KEY_F6);
    m_Fkeys.insert("KEY_F7", KEY_F7);
    m_Fkeys.insert("KEY_F8", KEY_F8);
    m_Fkeys.insert("KEY_F9", KEY_F9);
    m_Fkeys.insert("KEY_F10", KEY_F10);
    m_Fkeys.insert("KEY_F11", KEY_F11);
    m_Fkeys.insert("KEY_F12", KEY_F12);
    m_Fkeys.insert("KEY_LEFT", KEY_LEFT);
    m_Fkeys.insert("KEY_RIGHT", KEY_RIGHT);
    m_Fkeys.insert("KEY_UP", KEY_UP);
    m_Fkeys.insert("KEY_DOWN", KEY_DOWN);
    m_Fkeys.insert("KEY_PAGEUP", KEY_PAGEUP);
    m_Fkeys.insert("KEY_PAGEDOWN", KEY_PAGEDOWN);
    m_Fkeys.insert("KEY_ESC", KEY_ESC);
    m_Fkeys.insert("KEY_ENTER", KEY_ENTER);
}

void TcpServer::insert_text(QString t_text)
{
//    t_text = "hello1234567890-=!@#$%^&*()_+qwertyuiop[]\\|}{asdfghjk,nvxz<>/?";
    for(int i = 0; i < t_text.size(); i++) {
        QChar qch = t_text[i];
        char ch = qch.toLatin1();
        switch (ch) {
        case '!':
            push_shift_key_button(KEY_1); break;
        case '@':
            push_shift_key_button(KEY_2); break;
        case '#':
            push_shift_key_button(KEY_3); break;
        case '$':
            push_shift_key_button(KEY_4); break;
        case '%':
            push_shift_key_button(KEY_5); break;
        case '^':
            push_shift_key_button(KEY_6); break;
        case '&':
            push_shift_key_button(KEY_7); break;
        case '(':
            push_shift_key_button(KEY_9); break;
        case ')':
            push_shift_key_button(KEY_0); break;
        case '_':
            push_shift_key_button(KEY_MINUS); break;
        default:
            __U16_TYPE code = keys[t_text[i]];
            push_key_button(code);
        }
    }
}

void TcpServer::push_sequence_button(QString t_buttons)
{
    for(int i = 0; i < t_buttons.size(); i++) {
        QChar qch = t_buttons[i];
        char ch = qch.toLatin1();
        switch (ch) {
        case '!':
            push_shift_key_button(KEY_1); break;
        case '@':
            push_shift_key_button(KEY_2); break;
        case '#':
            push_shift_key_button(KEY_3); break;
        case '$':
            push_shift_key_button(KEY_4); break;
        case '%':
            push_shift_key_button(KEY_5); break;
        case '^':
            push_shift_key_button(KEY_6); break;
        case '&':
            push_shift_key_button(KEY_7); break;
        case '(':
            push_shift_key_button(KEY_9); break;
        case ')':
            push_shift_key_button(KEY_0); break;
        case '_':
            push_shift_key_button(KEY_MINUS); break;
        default:
            __U16_TYPE code = keys[t_buttons[i]];
            push_key_button(code);
        }
    }
}

void TcpServer::push_F_button(QString t_buttons)
{
    __U16_TYPE code = m_Fkeys[t_buttons];
    push_key_button(code);
}

void TcpServer::press_F_button(QString t_buttons)
{
    __U16_TYPE code = m_Fkeys[t_buttons];
    press_button_key(code);
}

void TcpServer::release_F_button(QString t_buttons)
{
    __U16_TYPE code = m_Fkeys[t_buttons];
    release_button_key(code);
}
