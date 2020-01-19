#include "main.h"
//#include <assert.h>




//#include <sys/syslog.h>
//#include <sys/socket.h>
//#include <netinet/in.h>
//#include <pthread.h>
//#include <iostream>
//#include <sstream>
//#include <vector>

using namespace std;



#define BUF_SIZE 1024
//static int ls;


void push_shift_key_button(__U16_TYPE code) {
//    struct input_event ev[8];
//    memset(&ev, 0, sizeof(ev));
//    ev[0].type = EV_KEY;
//    ev[0].code = KEY_LEFTSHIFT;
//    ev[0].value = 1;
//    ev[1].type = EV_SYN;
//    ev[1].code = SYN_REPORT;
//    ev[1].value = 0;
//    ev[2].type = EV_KEY;
//    ev[2].code = code;
//    ev[2].value = 1;
//    ev[3].type = EV_SYN;
//    ev[3].code = SYN_REPORT;
//    ev[3].value = 0;
//    ev[4].type = EV_KEY;
//    ev[4].code = code;
//    ev[4].value = 0;
//    ev[5].type = EV_SYN;
//    ev[5].code = SYN_REPORT;
//    ev[5].value = 0;
//    ev[6].type = EV_KEY;
//    ev[6].code = KEY_LEFTSHIFT;
//    ev[6].value = 0;
//    ev[7].type = EV_SYN;
//    ev[7].code = SYN_REPORT;
//    ev[7].value = 0;

//    if(write(fd, ev, sizeof(ev)) < 0) {
//        printf("error: ABS_Y-write");
//        return ;
//    }
}

void push_key_button(__U16_TYPE code) {
//    struct input_event ev[4];
//    memset(&ev, 0, sizeof(ev));
//    ev[0].type = EV_KEY;
//    ev[0].code = code;
//    ev[0].value = 1;
//    ev[1].type = EV_SYN;
//    ev[1].code = SYN_REPORT;
//    ev[1].value = 0;
//    ev[2].type = EV_KEY;
//    ev[2].code = code;
//    ev[2].value = 0;
//    ev[3].type = EV_SYN;
//    ev[3].code = SYN_REPORT;
//    ev[3].value = 0;
//    if(write(fd, ev, sizeof(ev)) < 0) {
//        printf("error: ABS_Y-write");
//        return ;
//    }
}

BOOL CALLBACK speichereFenster(HWND hwnd, LPARAM lParam){
    const DWORD TITLE_SIZE = 1024;
    WCHAR windowTitle[TITLE_SIZE];

    GetWindowTextW(hwnd, windowTitle, TITLE_SIZE);

    int length = ::GetWindowTextLength(hwnd);
    wstring title(&windowTitle[0]);
    if (!IsWindowVisible(hwnd) || length == 0 || title == L"Program Manager") {
        return TRUE;
    }

    // Retrieve the pointer passed into this callback, and re-'type' it.
    // The only way for a C API to pass arbitrary data is by means of a void*.
    std::vector<std::wstring>& titles =
                              *reinterpret_cast<std::vector<std::wstring>*>(lParam);
    titles.push_back(title);

    return TRUE;
}

void press_button_key(__U16_TYPE code) {
#ifdef __linux__
    struct input_event ev[2];
    memset(&ev, 0, sizeof(ev));
    ev[0].type = EV_KEY;
    ev[0].code = code;
    ev[0].value = 1;
    ev[1].type = EV_SYN;
    ev[1].code = SYN_REPORT;
    ev[1].value = 0;
    if(write(fd, ev, sizeof(ev)) < 0) {
        printf("error: sync-report");
        return ;
    }
#elif _WIN32



//    keybd_event('A', 0,0,0);
//    keybd_event('A', 0,KEYEVENTF_KEYUP,0);


    const UINT mappedKey = MapVirtualKey( code, 0 );
    INPUT buffer = {0};
    buffer.type = INPUT_KEYBOARD;
    buffer.ki.wVk = 0;
    buffer.ki.wScan = mappedKey;
    buffer.ki.dwFlags = KEYEVENTF_SCANCODE;
    buffer.ki.time = 0;
    buffer.ki.dwExtraInfo = 0;
    SendInput(1, &buffer, sizeof(INPUT));




//    INPUT buffer = {0};
//    buffer.type = INPUT_KEYBOARD;
//    buffer.ki = {0};
//    buffer.ki.wVk = static_cast<WORD>(code);
//    buffer.ki.wScan = 0;
//    buffer.ki.dwFlags = 0;
//    buffer.ki.time = 0;
//    buffer.ki.dwExtraInfo = 0;
//    SendInput(1, &buffer, sizeof(INPUT));


//    std::vector<std::wstring> titles;
//    EnumWindows(speichereFenster, reinterpret_cast<LPARAM>(&titles));
//    // Ast this point, titles if fully populated and could be displayed, e.g.:
//    for ( const auto& title : titles ) {
//        QString test = QString::fromWCharArray( title.c_str() );
//        qDebug() << test;
//    }

//    auto window = FindWindowA(NULL, "Black Desert - 346125");

//    DWORD key = static_cast<DWORD>('W');
//    SendMessage(window, WM_KEYDOWN, key, 0);
//    SendMessage(window, WM_CHAR, key, 1);
//    SendMessage(window, WM_KEYUP, key, 1);


//    buffer.ki.dwFlags = KEYEVENTF_KEYUP;
//    SendInput(1, &buffer, sizeof(INPUT));



    qDebug() << "press_button_key";
#endif
}

void release_button_key(__U16_TYPE code) {
#ifdef __linux__
    struct input_event ev[2];
    memset(&ev, 0, sizeof(ev));
    ev[0].type = EV_KEY;
    ev[0].code = code;
    ev[0].value = 0;
    ev[1].type = EV_SYN;
    ev[1].code = SYN_REPORT;
    ev[1].value = 0;
    if(write(fd, ev, sizeof(ev)) < 0) {
        printf("error: sync-report");
        return ;
    }
#elif _WIN32
    const UINT mappedKey = MapVirtualKey( code, 0 );
    INPUT buffer = {0};
    buffer.type = INPUT_KEYBOARD;
    buffer.ki.wVk = 0;
    buffer.ki.wScan = mappedKey;
    buffer.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
    buffer.ki.time = 0;
    buffer.ki.dwExtraInfo = 0;
    SendInput(1, &buffer, sizeof(INPUT));
    qDebug() << "release_button_key";
#endif
}


void mouse_click(__U16_TYPE code) {

#ifdef __linux__
    struct input_event ev[4];
    memset(&ev, 0, sizeof(ev));
    ev[0].type = EV_KEY;
    ev[0].code = code;
    ev[0].value = 1;
    ev[1].type = EV_SYN;
    ev[1].code = SYN_REPORT;
    ev[1].value = 0;
    ev[2].type = EV_KEY;
    ev[2].code = code;
    ev[2].value = 0;
    ev[3].type = EV_SYN;
    ev[3].code = SYN_REPORT;
    ev[3].value = 0;
    if(write(fd, ev, sizeof(ev)) < 0) {
        printf("error: ABS_Y-write");
        return ;
    }
#elif _WIN32
    INPUT buffer = {0};
    buffer.type = INPUT_MOUSE;
    buffer.mi.mouseData = 0;
    buffer.mi.time = 0;
    buffer.mi.dwExtraInfo = 0;
    if(code == MOUSEEVENTF_LEFTDOWN) {
        buffer.mi.dwFlags = code;
        SendInput(1, &buffer, sizeof(buffer));
        QThread::msleep(15);
        buffer.mi.dwFlags = MOUSEEVENTF_LEFTUP;
        SendInput(1, &buffer, sizeof(buffer));
    } else if (code == MOUSEEVENTF_RIGHTDOWN) {
        buffer.mi.dwFlags = code;
        SendInput(1, &buffer, sizeof(buffer));
        QThread::msleep(15);
        buffer.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
        SendInput(1, &buffer, sizeof(buffer));
    }
    qDebug() << "mouse_click";
#endif

}

void mouse_move_click(int x, int y, __U16_TYPE code) {
#ifdef __linux__
    struct input_event ev[7];
    memset(&ev, 0, sizeof(ev));
    ev[0].type = EV_ABS;
    ev[0].code = ABS_X;
    ev[0].value = x;
    ev[1].type = EV_ABS;
    ev[1].code = ABS_Y;
    ev[1].value = y ;
    ev[2].type = EV_SYN;
    ev[2].code = SYN_REPORT;
    ev[2].value = 0;
    ev[3].type = EV_KEY;
    ev[3].code = code;
    ev[3].value = 1;
    ev[4].type = EV_SYN;
    ev[4].code = SYN_REPORT;
    ev[4].value = 0;
    ev[5].type = EV_KEY;
    ev[5].code = code;
    ev[5].value = 0;
    ev[6].type = EV_SYN;
    ev[6].code = SYN_REPORT;
    ev[6].value = 0;

    if(write(fd, ev, sizeof(ev)) < 0) {
        printf("error: ABS_Y-write");
        return ;
    }
#elif _WIN32
    INPUT buffer = {0};
    buffer.type = INPUT_MOUSE;
    buffer.mi.mouseData = 0;
    buffer.mi.time = 0;
    buffer.mi.dwExtraInfo = 0;
    buffer.mi.dx = (x * (0xFFFF / sizeScreen.width()));
    buffer.mi.dy = (y * (0xFFFF / sizeScreen.height()));
    buffer.mi.dwFlags = (MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE);
    SendInput(1, &buffer, sizeof(INPUT));

    if(code == MOUSEEVENTF_LEFTDOWN) {
        buffer.mi.dwFlags = code;
        SendInput(1, &buffer, sizeof(buffer));
        QThread::msleep(15);
        buffer.mi.dwFlags = MOUSEEVENTF_LEFTUP;
        SendInput(1, &buffer, sizeof(buffer));
    } else if (code == MOUSEEVENTF_RIGHTDOWN) {
        buffer.mi.dwFlags = code;
        SendInput(1, &buffer, sizeof(buffer));
        QThread::msleep(15);
        buffer.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
        SendInput(1, &buffer, sizeof(buffer));
    }
    qDebug() << "mouse_move_click";

#endif

}

void mouse_move_press(int x, int y, __U16_TYPE code) {
#ifdef __linux__
    struct input_event ev[4];
    memset(&ev, 0, sizeof(ev));
    ev[0].type = EV_ABS;
    ev[0].code = ABS_X;
    ev[0].value = x;
    ev[1].type = EV_ABS;
    ev[1].code = ABS_Y;
    ev[1].value = y ;
    ev[2].type = EV_SYN;
    ev[2].code = SYN_REPORT;
    ev[2].value = 0;
    ev[3].type = EV_KEY;
    ev[3].code = code;
    ev[3].value = 1;
    if(write(fd, ev, sizeof(ev)) < 0) {
        printf("error: ABS_Y-write");
        return ;
    }
#elif _WIN32
    INPUT buffer = {0};
    buffer.type = INPUT_MOUSE;
    buffer.mi.mouseData = 0;
    buffer.mi.time = 0;
    buffer.mi.dwExtraInfo = 0;
    buffer.mi.dx = (x * (0xFFFF / sizeScreen.width()));
    buffer.mi.dy = (y * (0xFFFF / sizeScreen.height()));
    buffer.mi.dwFlags = (MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE);
    SendInput(1, &buffer, sizeof(INPUT));

    INPUT buffer2 = {0};
    buffer2.type = INPUT_MOUSE;
    buffer2.mi.mouseData = 0;
    buffer2.mi.time = 0;
    buffer2.mi.dwExtraInfo = 0;
    buffer2.mi.dwFlags = code;
    SendInput(1, &buffer2, sizeof(buffer2));
    qDebug() << "mouse_move_press";

#endif

}

void mouse_move_release(int x, int y, __U16_TYPE code)
{
#ifdef __linux__
    struct input_event ev[4];
    memset(&ev, 0, sizeof(ev));
    ev[0].type = EV_ABS;
    ev[0].code = ABS_X;
    ev[0].value = x;
    ev[1].type = EV_ABS;
    ev[1].code = ABS_Y;
    ev[1].value = y ;
    ev[2].type = EV_SYN;
    ev[2].code = SYN_REPORT;
    ev[2].value = 0;
    ev[3].type = EV_KEY;
    ev[3].code = code;
    ev[3].value = 0;
    if(write(fd, ev, sizeof(ev)) < 0) {
        printf("error: ABS_Y-write");
        return ;
    }
#elif _WIN32
    INPUT buffer = {0};
    buffer.type = INPUT_MOUSE;
    buffer.mi.mouseData = 0;
    buffer.mi.time = 0;
    buffer.mi.dwExtraInfo = 0;
    buffer.mi.dx = (x * (0xFFFF / sizeScreen.width()));
    buffer.mi.dy = (y * (0xFFFF / sizeScreen.height()));
    buffer.mi.dwFlags = (MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE);
    SendInput(1, &buffer, sizeof(INPUT));

    INPUT buffer2 = {0};
    buffer2.type = INPUT_MOUSE;
    buffer2.mi.mouseData = 0;
    buffer2.mi.time = 0;
    buffer2.mi.dwExtraInfo = 0;
    buffer2.mi.dwFlags = code;
    SendInput(1, &buffer2, sizeof(buffer2));
    qDebug() << "mouse_move_release";

#endif

}

void mouse_move(int x, int y, QString rel) {
#ifdef __linux__
    struct input_event ev[3];
    memset(&ev, 0, sizeof(ev));
    ev[0].type = EV_ABS;
    ev[0].code = ABS_X;
    ev[0].value = x;
    ev[1].type = EV_ABS;
    ev[1].code = ABS_Y;
    ev[1].value = y ;
    ev[2].type = EV_SYN;
    ev[2].code = SYN_REPORT;
    ev[2].value = 0;

    if(write(fd, ev, sizeof(ev)) < 0)
    {
        printf("error: ABS_Y-write");
        return ;
    }
#elif _WIN32
    INPUT buffer;
    buffer.type = INPUT_MOUSE;
    buffer.mi.mouseData = 0;
    buffer.mi.time = 0;
    buffer.mi.dwExtraInfo = 0;
    if(rel == "") {
        buffer.mi.dx = (x * (0xFFFF / sizeScreen.width()));
        buffer.mi.dy = (y * (0xFFFF / sizeScreen.height()));
        buffer.mi.dwFlags = (MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE);
    } else if(rel == "REL"){
        buffer.mi.dx = x;
        buffer.mi.dy = y;
        buffer.mi.dwFlags = MOUSEEVENTF_MOVE;
    }

    SendInput(1, &buffer, sizeof(INPUT));
    qDebug() << "mouse_move";

#endif
}

void destroy_app(){
//    usleep(_wait);
//    if(ioctl(fd, UI_DEV_DESTROY) < 0)
//    {
//        printf("error: ioctl");
//        return ;
//    }
//    close(fd);
}

void send_syn() {
//    struct input_event evS;
//    memset(&evS, 0, sizeof(struct input_event));
//    evS.type = EV_SYN;
//    evS.code = SYN_REPORT;
//    evS.value = 0;
//    if(write(fd, &evS, sizeof(struct input_event)) < 0) {
//        printf("error: sync-report");
//        return ;
//    }
}

#ifdef __linux__
int init() {

    fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK); //opening of uinput
    if (fd < 0) {
        printf("Opening of uinput failed!\n");
        return 1;
    }

    ioctl(fd, UI_SET_KEYBIT, BTN_RIGHT);
    ioctl(fd, UI_SET_KEYBIT, BTN_LEFT);
    ioctl(fd, UI_SET_EVBIT, EV_KEY);
    ioctl(fd, UI_SET_EVBIT, EV_ABS);
    ioctl(fd, UI_SET_ABSBIT, ABS_X);
    ioctl(fd, UI_SET_ABSBIT, ABS_Y);

    for(int i = 1; i < 80; i++)
        ioctl(fd, UI_SET_KEYBIT, i);


    struct uinput_user_dev uidev;
    memset(&uidev, 0, sizeof(uidev));
    snprintf(uidev.name, UINPUT_MAX_NAME_SIZE, "HolusionMouse"); //Name of Gamepad
    uidev.id.bustype = BUS_USB;
    uidev.id.vendor  = 0x33a3;
    uidev.id.product = 0x3b33;
    uidev.id.version = 222;


    Display* d = XOpenDisplay(nullptr);
    Screen*  s = DefaultScreenOfDisplay(d);

    uidev.absmin[ABS_X] = 0;
    uidev.absmax[ABS_X] = s->width;

    uidev.absmin[ABS_Y] = 0;
    uidev.absmax[ABS_Y] = s->height;

    printf("width=%d  height=%d\n", s->width, s->height);
    fflush(stdout);


    if(write(fd, &uidev, sizeof(uidev)) < 0) //writing settings
    {
        printf("error: write");
        return 1;
    }

    if(ioctl(fd, UI_DEV_CREATE) < 0) //writing ui dev create
    {
        printf("error: ui_dev_create");
        return 1;
    }
    usleep(_wait);
    return fd;
}

#elif _WIN32
int init() {
//    QList<QScreen *> screens;

//    screens = QApplication::screens();
    sizeScreen = qApp->primaryScreen()->size();
//    QSize sizeScreen = qApp->screens()[0]->size();

//    buffer->type = INPUT_MOUSE;
//    buffer->mi.dx = (0 * (0xFFFF / sizeScreen.width()));
//    buffer->mi.dy = (0 * (0xFFFF / sizeScreen.height()));
//    buffer->mi.mouseData = 0;
//    buffer->mi.dwFlags = MOUSEEVENTF_ABSOLUTE;
//    buffer->mi.time = 0;
//    buffer->mi.dwExtraInfo = 0;
    return 0;
}
#endif

int main(int argc, char **argv)
{
//    QCoreApplication a(argc, argv);
    QApplication a(argc, argv);
#ifdef __linux__
    if(init() == 1)
        return 1;
    //    press_key(KEY_Z);
    //    input_click_up(300, 300, BTN_LEFT);
    //    release_button(BTN_LEFT);
    //    msleep(_wait);

#elif _WIN32
    if(init() == 1)
        return 1;


//    mouse_move(100, 100);

#endif




    TcpServer server;

    return a.exec();
}





/*
//std::vector<std::string> getListCMD(char *buff);
//typedef std::vector<std::string> list;

//std::vector<std::string> getListCMD(std::string &s){
//    stringstream str(s);
//    std::string segment;
//    std::vector<std::string> seglist;

//    while(std::getline(str, segment, ':'))
//    {
//       seglist.push_back(segment);
//    }
//    return seglist;
//}

//void parseScrypt(std::vector<std::string>* script){
//    for(std::string str: *script) {
//        list line =getListCMD(str);
//        if(line[0] == "release_button") {
//            if(line[1] == "BTN_LEFT") {
//                release_button(BTN_LEFT);
//            } else if(line[1] == "BTN_RIGHT") {
//                release_button(BTN_RIGHT);
//            }
//        } else if(line[0] == "move_cursor") {
//            move_cursor(std::stoi(line[1]), std::stoi(line[2]));
//        } else if(line[0] == "input_click") {
//            __U16_TYPE code = line[3]=="BTN_LEFT" ? BTN_LEFT : BTN_RIGHT;
//            input_click(std::stoi(line[1]), std::stoi(line[2]), code);
//        } else if(line[0] == "send_syn") {
//            send_syn();
//        }

//    }
//}

//std::vector<std::string> getListCMD(char *buff){
//    stringstream str;
//    str << static_cast<const char*>(buff);

//    std::string segment;
//    std::vector<std::string> seglist;

//    while(std::getline(str, segment, ','))
//    {
//       seglist.push_back(segment);
//    }
//    return seglist;
//}

//void f(int rc) {
//    char buff[BUF_SIZE];
//    ssize_t n = 0;
//    while ( (n = read(rc, buff, sizeof(buff)-1)) > 0)
//    {
//        buff[n] = 0;
//        if(fputs(buff, stdout) == EOF)
//        {
//            printf("\n Error : Fputs error\n");
//        }
//    }
//    std::vector<std::string> script = getListCMD(buff);
//    parseScrypt(&script);
//}

//void start_server(){
//    struct sockaddr_in addr;
////    struct sockaddr_un local;
//    int rc;
//    __pid_t pid;
//    if((ls = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP) ) == -1) {
//        perror("Socket con not created!\n");
//        return;
//    }
//    int setsockopt(int socket, int level, int option_name, const void *option_value, socklen_t option_len);
//    memset(&addr, 0, sizeof (0));
//    addr.sin_family = AF_INET;
//    addr.sin_port = htons (10101);
//    addr.sin_addr.s_addr = INADDR_ANY ;
//    if(bind(ls, reinterpret_cast<struct sockaddr *> (&addr), sizeof (addr)) < 0) {
//        close(ls);
//        printf("bind error!\n");
//        return;
//    }
//    if(listen(ls, 25) < 0) {
//        close(ls);
//        perror("listen error\n");
//        return;
//    }
////    pid_t pid = Demonize();
//    while (true) {
//        rc = accept(ls, nullptr, nullptr);
//        f(rc);
//        close(rc);

////        pid = fork();
////        if ( pid < 0) syslog( LOG_ERR, " fork abort" );
////        if(pid == 0) {
////            close(ls);
////            f(rc);
////            close(rc);
////            return;
////        } else {
////            close(rc);

////        }
//    }
//}
*/
