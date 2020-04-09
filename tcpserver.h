#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QCoreApplication>
#include <QObject>
#include <QDebug>
#include <QTcpServer>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QPushButton>
#include <QThread>
#include <QLayout>
#include <QJsonObject>
#include <QJsonDocument>

#ifdef __linux__
#include <linux/uinput.h>
#include <X11/Xlib.h>
static int fd;
typedef unsigned __int16 __U16_TYPE;


#elif _WIN32
#include <windows.h>
#include <QApplication>
#include <QWidget>
#include <QScreen>
#include <QLabel>
#include <QTextEdit>
//static INPUT buffer[1];
static QSize sizeScreen;
typedef DWORD __U16_TYPE;
#endif

class TcpServer : public QWidget
{
    Q_OBJECT
    QTcpServer *tcpServer = nullptr;

    void slotSendMessage(QTcpSocket *socket, QString t_message);
    void initKeyButton();
    void initFKeyButton();
    void insert_text(QString t_text);
    void push_sequence_button(QString t_buttons);
    void push_F_button(QString t_buttons, uint = 75);
    void press_F_button(QString t_buttons);
    void release_F_button(QString t_buttons);
    QMap <QString, __U16_TYPE> keys;
    QMap <QString, __U16_TYPE> m_Fkeys;

    int m_counter = 0;

    QVBoxLayout *vblayout;
    QTextEdit *text;
public:
    explicit TcpServer(QWidget *parent = nullptr);
    ~TcpServer();

    void setText(QString aStr);

signals:

private slots:
    void slotReadyRead();
    void slotNewConnection();
};

#endif // TCPSERVER_H
