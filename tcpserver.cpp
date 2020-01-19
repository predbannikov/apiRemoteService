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
#ifdef __linux__
    __U16_TYPE code = BTN_LEFT;
#elif _WIN32
    __U16_TYPE code = MOUSEEVENTF_LEFTDOWN;
#endif

#ifdef __linux__
#elif _WIN32
#endif


    if(_jObj["target"].toString() == "keyboard") {                          // **************** KEYBOARD *****************
        if(_jObj["method"].toString() == "type") {                          // type
            push_sequence_button(_jObj["text"].toString()); //!
        } else if(_jObj["method"].toString() == "push_f") {                 // push functional key
            push_F_button(_jObj["f_code"].toString());
        } else if(_jObj["method"].toString() == "press_f") {                // press functional key
            press_F_button(_jObj["f_code"].toString());
        } else if(_jObj["method"].toString() == "release_f") {              // release functional key
            release_F_button(_jObj["f_code"].toString());
        } else if(_jObj["method"].toString() == "press") {                  // press key
            QChar _qch = _jObj["code"].toString()[0];
            press_button_key(keys[_qch]);
        } else if(_jObj["method"].toString() == "release") {                // release key
            QChar _qch = _jObj["code"].toString()[0];
            release_button_key(keys[_qch]);
        }
    } else if(_jObj["target"].toString() == "mouse") {                      // **************** MOUSE *****************
        if(_jObj["method"].toString() == "move") {                                     // move
            if(_jObj["move_type"].toString() == "")
                mouse_move(_jObj["x"].toInt(), _jObj["y"].toInt());
            else if(_jObj["move_type"].toString() == "REL")
                mouse_move(_jObj["x"].toInt(), _jObj["y"].toInt(), _jObj["move_type"].toString());
        } else if(_jObj["method"].toString() == "move_click") {             // move and click
#ifdef __linux__
            code =_jObj["code"].toString() == "BTN_LEFT" ? BTN_LEFT : BTN_RIGHT;
#elif _WIN32
            code =_jObj["code"].toString() == "BTN_LEFT" ? MOUSEEVENTF_LEFTDOWN : MOUSEEVENTF_RIGHTDOWN;
#endif
            mouse_move_click(_jObj["x"].toInt(), _jObj["y"].toInt(), code);
        } else if(_jObj["method"].toString() == "click") {                  // click
#ifdef __linux__
            code =_jObj["code"].toString() == "BTN_LEFT" ? BTN_LEFT : BTN_RIGHT;
#elif _WIN32
            code =_jObj["code"].toString() == "BTN_LEFT" ? MOUSEEVENTF_LEFTDOWN : MOUSEEVENTF_RIGHTDOWN;
#endif
            mouse_click(code);
        } else if(_jObj["method"].toString() == "move_press") {             // move and press
#ifdef __linux__
            code =_jObj["code"].toString() == "BTN_LEFT" ? BTN_LEFT : BTN_RIGHT;
#elif _WIN32
            code =_jObj["code"].toString() == "BTN_LEFT" ? MOUSEEVENTF_LEFTDOWN : MOUSEEVENTF_RIGHTDOWN;
#endif
            mouse_move_press(_jObj["x"].toInt(), _jObj["y"].toInt(), code);
        } else if(_jObj["method"].toString() == "move_release") {           // move and release
#ifdef __linux__
            code =_jObj["code"].toString() == "BTN_LEFT" ? BTN_LEFT : BTN_RIGHT;
#elif _WIN32
            code =_jObj["code"].toString() == "BTN_LEFT" ? MOUSEEVENTF_LEFTUP : MOUSEEVENTF_RIGHTUP;
#endif
            mouse_move_release(_jObj["x"].toInt(), _jObj["y"].toInt(), code);
        }
    } else if(_jObj["target"].toString() == "server") {                     // **************** SERVER *****************
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

void TcpServer::initFKeyButton()
{
#ifdef __linux__
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
#elif _WIN32

#endif
}

void TcpServer::initKeyButton()
{
#ifdef __linux__
    // Mouse
    //--

    //Numbers
    keys.insert("0", KEY_0);
    keys.insert("1", KEY_1);
    keys.insert("2", KEY_2);
    keys.insert("3", KEY_3);
    keys.insert("4", KEY_4);
    keys.insert("5", KEY_5);
    keys.insert("6", KEY_6);
    keys.insert("7", KEY_7);
    keys.insert("8", KEY_8);
    keys.insert("9", KEY_9);

    //Letters
    keys.insert("Q", KEY_Q);
    keys.insert("q", KEY_Q);
    keys.insert("W", KEY_W);
    keys.insert("w", KEY_W);
    keys.insert("E", KEY_E);
    keys.insert("e", KEY_E);
    keys.insert("R", KEY_R);
    keys.insert("r", KEY_R);
    keys.insert("T", KEY_T);
    keys.insert("t", KEY_T);
    keys.insert("Y", KEY_Y);
    keys.insert("y", KEY_Y);
    keys.insert("U", KEY_U);
    keys.insert("u", KEY_U);
    keys.insert("I", KEY_I);
    keys.insert("i", KEY_I);
    keys.insert("O", KEY_O);
    keys.insert("o", KEY_O);
    keys.insert("P", KEY_P);
    keys.insert("p", KEY_P);
    keys.insert("a", KEY_A);
    keys.insert("A", KEY_A);
    keys.insert("s", KEY_S);
    keys.insert("S", KEY_S);
    keys.insert("d", KEY_D);
    keys.insert("D", KEY_D);
    keys.insert("F", KEY_F);
    keys.insert("f", KEY_F);
    keys.insert("G", KEY_G);
    keys.insert("g", KEY_G);
    keys.insert("h", KEY_H);
    keys.insert("H", KEY_H);
    keys.insert("j", KEY_J);
    keys.insert("J", KEY_J);
    keys.insert("k", KEY_K);
    keys.insert("K", KEY_K);
    keys.insert("L", KEY_L);
    keys.insert("l", KEY_L);
    keys.insert("z", KEY_Z);
    keys.insert("Z", KEY_Z);
    keys.insert("X", KEY_X);
    keys.insert("x", KEY_X);
    keys.insert("C", KEY_C);
    keys.insert("c", KEY_C);
    keys.insert("V", KEY_V);
    keys.insert("v", KEY_V);
    keys.insert("B", KEY_B);
    keys.insert("b", KEY_B);
    keys.insert("N", KEY_N);
    keys.insert("n", KEY_N);
    keys.insert("M", KEY_M);
    keys.insert("m", KEY_M);

    // Other symbols
    keys.insert(" ", KEY_SPACE);
    keys.insert(".", KEY_DOT);
    keys.insert("*", KEY_KPASTERISK);
    keys.insert("\"', KEY_APOSTROPHE);
    keys.insert(";", KEY_SEMICOLON);
    keys.insert("]", KEY_RIGHTBRACE);
    keys.insert("[", KEY_LEFTBRACE);
    keys.insert(",", KEY_COMMA);
    keys.insert("=", KEY_EQUAL);
    keys.insert("-", KEY_MINUS);
    keys.insert("+", KEY_KPPLUS);
    keys.insert("`", KEY_GRAVE);
    keys.insert("/", KEY_SLASH);
    keys.insert("\\", KEY_BACKSLASH);

    // F keys
    keys.insert("F1", KEY_F1);
    keys.insert("F2", KEY_F2);
    keys.insert("F3", KEY_F3);
    keys.insert("F4", KEY_F4);
    keys.insert("F5", KEY_F5);
    keys.insert("F6", KEY_F6);
    keys.insert("F7", KEY_F7);
    keys.insert("F8", KEY_F8);
    keys.insert("F9", KEY_F9);
    keys.insert("F10", KEY_F10);
    keys.insert("F11", KEY_F11);
    keys.insert("F12", KEY_F12);

    // Other keys
    keys.insert("LEFT", KEY_LEFT);
    keys.insert("RIGHT", KEY_RIGHT);
    keys.insert("UP", KEY_UP);
    keys.insert("DOWN", KEY_DOWN);
    keys.insert("PAGEUP", KEY_PAGEUP);
    keys.insert("PAGEDOWN", KEY_PAGEDOWN);
    keys.insert("ESC", KEY_ESC);
    keys.insert("ENTER", KEY_ENTER);

#elif _WIN32

    // Mouse
    keys.insert("LMB", 0x01); // Left mouse button
    keys.insert("RMB", 0x02); // Right mouse button
    keys.insert("MMB", 0x04); // Middle mouse button
    keys.insert("X1MB", 0x05); // X1 mouse button
    keys.insert("X2MB", 0x06); // X2 mouse button

    // Other symbols
    keys.insert(" ", 0x20); // SPACEBAR
    keys.insert("*", 0x6A); // MULTIPLY
    //keys.insert("+", 0x6B); // ADD
    //keys.insert("", 0x6C); // SEPARATOR KEY (не в курсе, что за кнопка, но она на нам паде)
    keys.insert("-", 0x6D); // SUBTRACT KEY
    //keys.insert(".", 0x6E); // DECIMAL KEY (не уверен, что за кнопка)
    //keys.insert("/", 0x6F); // DIVIDE KEY (не уверен, что за кнопка)

    // Better than some of other symbols
    keys.insert("+", 0xBB); // '+' KEY FOR ANY COUNTRY/REGION
    keys.insert(",", 0xBC); // ',' KEY FOR ANY COUNTRY/REGION
    keys.insert("-", 0xBD); // '-' KEY FOR ANY COUNTRY/REGION
    keys.insert(".", 0xBE); // '.' KEY FOR ANY COUNTRY/REGION

    // Can vary by keyboard
    keys.insert("/?", 0xBF); // '/?' KEY CAN VARY BY KEYBOARD
    keys.insert("`~", 0xC0); // '`~' KEY CAN VARY BY KEYBOARD
    keys.insert("[{", 0xDB); // '[{' KEY CAN VARY BY KEYBOARD
    keys.insert("\\|", 0xDC); // '\|' KEY CAN VARY BY KEYBOARD
    keys.insert("]}", 0xDD); // ']}' KEY CAN VARY BY KEYBOARD
    keys.insert("\'\"", 0xDE); // ' '" ' KEY CAN VARY BY KEYBOARD

    // Other (не включал IME-коды. Они используются для иероглифных клавиатур для переключения их раскладок), (не включал кнопки управления браузером, звуком, музыкой)
    //keys.insert('', 0x03); // Control-break processing - ???
    keys.insert("BACKSPACE", 0x08); // BACKSPACE
    keys.insert("TAB", 0x09); // TAB
    keys.insert("CLEAR", 0x0C); // CLEAR - ???
    keys.insert("ENTER", 0x0D); // ENTER
    keys.insert("SHIFT", 0x10); // SHIFT
    keys.insert("CTRL", 0x11); // CONTROL
    keys.insert("ALT", 0x12); // ALT
    keys.insert("PAUSE", 0x13); // PAUSE
    keys.insert("CAPS_LOCK", 0x14); // CAPS LOCK
    keys.insert("ESC", 0x1B); // ESC
    keys.insert("PAGE_UP", 0x21); // PAGE UP
    keys.insert("PAGE_DOWN", 0x22); // PAGE DOWN
    keys.insert("END", 0x23); // END
    keys.insert("HOME", 0x24); // HOME
    keys.insert("LEFT_ARROW", 0x25); // LEFT ARROW
    keys.insert("UP_ARROW", 0x26); // UP ARROW
    keys.insert("RIGHT_ARROW", 0x27); // RIGHT ARROW
    keys.insert("DOWN_ARROW", 0x28); // DOWN ARROW
    //keys.insert('', 0x29); // SELECT - ???
    //keys.insert('', 0x2A); // PRINT - ???
    //keys.insert('', 0x2B); // EXECUTE - ???
    keys.insert("PRINT_SCREEN", 0x2C); // PRINT SCREEN
    keys.insert("INS", 0x2D); // INS
    keys.insert("DEL", 0x2E); // DEL
    //keys.insert('', 0x2F); // HELP - ???
    keys.insert("LEFT_WIN", 0x5B); // LEFT WINDOWS KEY
    keys.insert("RIGHT_WIN", 0x5C); // RIGHT WINDOWS KEY
    keys.insert("APPS", 0x5D); // APPLICATIONS KEY
    keys.insert("SLEEP", 0x5F); // COMPUTER SLEEP KEY
    keys.insert("NUM_LOCK", 0x90); // NUM LOCK
    keys.insert("SCROLL_LOCK", 0x91); // SCROLL LOCK
    keys.insert("LSHIFT", 0xA0); // LEFT SHIFT
    keys.insert("RSHIFT", 0xA1); // RIGHT SHIFT
    keys.insert("LCTRL", 0xA2); // LEFT CONTROL
    keys.insert("RCTRL", 0xA3); // RIGHT CONTROL
    keys.insert("LMENU", 0xA4); // LEFT MENU
    keys.insert("RMENU", 0xA5); // RIGHT MENU

    // Numpad
    keys.insert("NUM_0", 0x60); // NUMERIC KEYPAD 0 KEY
    keys.insert("NUM_1", 0x61); // NUMERIC KEYPAD 1 KEY
    keys.insert("NUM_2", 0x62); // NUMERIC KEYPAD 2 KEY
    keys.insert("NUM_3", 0x63); // NUMERIC KEYPAD 3 KEY
    keys.insert("NUM_4", 0x64); // NUMERIC KEYPAD 4 KEY
    keys.insert("NUM_5", 0x65); // NUMERIC KEYPAD 5 KEY
    keys.insert("NUM_6", 0x66); // NUMERIC KEYPAD 6 KEY
    keys.insert("NUM_7", 0x67); // NUMERIC KEYPAD 7 KEY
    keys.insert("NUM_8", 0x68); // NUMERIC KEYPAD 8 KEY
    keys.insert("NUM_9", 0x69); // NUMERIC KEYPAD 9 KEY

    // Numbers
    keys.insert("0", 0x30);
    keys.insert("1", 0x31);
    keys.insert("2", 0x32);
    keys.insert("3", 0x33);
    keys.insert("4", 0x34);
    keys.insert("5", 0x35);
    keys.insert("6", 0x36);
    keys.insert("7", 0x37);
    keys.insert("8", 0x38);
    keys.insert("9", 0x39);

    // Letters
    keys.insert("a", 0x41);
    keys.insert("A", 0x41);
    keys.insert("b", 0x42);
    keys.insert("B", 0x42);
    keys.insert("c", 0x43);
    keys.insert("C", 0x43);
    keys.insert("d", 0x44);
    keys.insert("D", 0x44);
    keys.insert("e", 0x45);
    keys.insert("E", 0x45);
    keys.insert("f", 0x46);
    keys.insert("F", 0x46);
    keys.insert("g", 0x47);
    keys.insert("G", 0x47);
    keys.insert("h", 0x48);
    keys.insert("H", 0x48);
    keys.insert("i", 0x49);
    keys.insert("I", 0x49);
    keys.insert("j", 0x4A);
    keys.insert("J", 0x4A);
    keys.insert("k", 0x4B);
    keys.insert("K", 0x4B);
    keys.insert("l", 0x4C);
    keys.insert("L", 0x4C);
    keys.insert("m", 0x4D);
    keys.insert("M", 0x4D);
    keys.insert("n", 0x4E);
    keys.insert("N", 0x4E);
    keys.insert("o", 0x4F);
    keys.insert("O", 0x4F);
    keys.insert("p", 0x50);
    keys.insert("P", 0x50);
    keys.insert("q", 0x51);
    keys.insert("Q", 0x51);
    keys.insert("r", 0x52);
    keys.insert("R", 0x52);
    keys.insert("s", 0x53);
    keys.insert("S", 0x53);
    keys.insert("t", 0x54);
    keys.insert("T", 0x54);
    keys.insert("u", 0x55);
    keys.insert("U", 0x55);
    keys.insert("v", 0x56);
    keys.insert("V", 0x56);
    keys.insert("w", 0x57);
    keys.insert("W", 0x57);
    keys.insert("x", 0x58);
    keys.insert("X", 0x58);
    keys.insert("y", 0x59);
    keys.insert("Y", 0x59);
    keys.insert("z", 0x5A);
    keys.insert("Z", 0x5A);

    // F keys
    keys.insert("F1", 0x70); // F1
    keys.insert("F2", 0x71); // F2
    keys.insert("F3", 0x72); // F3
    keys.insert("F4", 0x73); // F4
    keys.insert("F5", 0x74); // F5
    keys.insert("F6", 0x75); // F6
    keys.insert("F7", 0x76); // F7
    keys.insert("F8", 0x77); // F8
    keys.insert("F9", 0x78); // F9
    keys.insert("F10", 0x79); // F10
    keys.insert("F11", 0x7A); // F11
    keys.insert("F12", 0x7B); // F12
    keys.insert("F13", 0x7C); // F13
    keys.insert("F14", 0x7D); // F14
    keys.insert("F15", 0x7E); // F15
    keys.insert("F16", 0x7F); // F16
    keys.insert("F17", 0x80); // F17
    keys.insert("F18", 0x81); // F18
    keys.insert("F19", 0x82); // F19
    keys.insert("F20", 0x83); // F20
    keys.insert("F21", 0x84); // F21
    keys.insert("F22", 0x85); // F22
    keys.insert("F23", 0x86); // F23
    keys.insert("F24", 0x87); // F24
#endif
}


void TcpServer::insert_text(QString t_text)
{
//    t_text = "hello1234567890-=!@#$%^&*()_+qwertyuiop[]\\|}{asdfghjk,nvxz<>/?";
//    for(int i = 0; i < t_text.size(); i++) {
//        QChar qch = t_text[i];
//        char ch = qch.toLatin1();
//        switch (ch) {
//        case '!':
//            push_shift_key_button(KEY_1); break;
//        case '@':
//            push_shift_key_button(KEY_2); break;
//        case '#':
//            push_shift_key_button(KEY_3); break;
//        case '$':
//            push_shift_key_button(KEY_4); break;
//        case '%':
//            push_shift_key_button(KEY_5); break;
//        case '^':
//            push_shift_key_button(KEY_6); break;
//        case '&':
//            push_shift_key_button(KEY_7); break;
//        case '(':
//            push_shift_key_button(KEY_9); break;
//        case ')':
//            push_shift_key_button(KEY_0); break;
//        case '_':
//            push_shift_key_button(KEY_MINUS); break;
//        default:
//            __U16_TYPE code = keys[t_text[i]];
//            push_key_button(code);
//        }
//    }
}

void TcpServer::push_sequence_button(QString t_buttons)
{
//    for(int i = 0; i < t_buttons.size(); i++) {
//        QChar qch = t_buttons[i];
//        char ch = qch.toLatin1();
//        switch (ch) {
//        case '!':
//            push_shift_key_button(KEY_1); break;
//        case '@':
//            push_shift_key_button(KEY_2); break;
//        case '#':
//            push_shift_key_button(KEY_3); break;
//        case '$':
//            push_shift_key_button(KEY_4); break;
//        case '%':
//            push_shift_key_button(KEY_5); break;
//        case '^':
//            push_shift_key_button(KEY_6); break;
//        case '&':
//            push_shift_key_button(KEY_7); break;
//        case '(':
//            push_shift_key_button(KEY_9); break;
//        case ')':
//            push_shift_key_button(KEY_0); break;
//        case '_':
//            push_shift_key_button(KEY_MINUS); break;
//        default:
//            __U16_TYPE code = keys[t_buttons[i]];
//            push_key_button(code);
//        }
//    }
}

void TcpServer::push_F_button(QString t_buttons)
{
#ifdef __linux__
    __U16_TYPE code = m_Fkeys[t_buttons];
    push_key_button(code);
#elif _WIN32
    __U16_TYPE code = keys[t_buttons];
    push_key_button(code);
#endif

}

void TcpServer::press_F_button(QString t_buttons)
{
#ifdef __linux__
    __U16_TYPE code = m_Fkeys[t_buttons];
    press_button_key(code);
#elif _WIN32
    __U16_TYPE code = keys[t_buttons];
    press_button_key(code);
#endif

}

void TcpServer::release_F_button(QString t_buttons)
{
#ifdef __linux__
    __U16_TYPE code = m_Fkeys[t_buttons];
    release_button_key(code);
#elif _WIN32
    __U16_TYPE code = keys[t_buttons];
    release_button_key(code);
#endif
}
