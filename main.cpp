#include "main.h"
//#include <assert.h>
#include "tcpserver.h"
#include <X11/Xlib.h>

//#include <sys/syslog.h>
//#include <sys/socket.h>
//#include <netinet/in.h>
//#include <pthread.h>
//#include <iostream>
//#include <sstream>
//#include <vector>

using namespace std;



#define BUF_SIZE 1024
static int fd;
//static int ls;


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



void destroy_app(){
    usleep(_wait);
    if(ioctl(fd, UI_DEV_DESTROY) < 0)
    {
        printf("error: ioctl");
        return ;
    }
    close(fd);
}

void send_syn() {
    struct input_event evS;
    memset(&evS, 0, sizeof(struct input_event));
    evS.type = EV_SYN;
//    evS.code = SYN_REPORT;
//    evS.value = 0;
    if(write(fd, &evS, sizeof(struct input_event)) < 0) {
        printf("error: sync-report");
        return ;
    }
}

void release_button(__U16_TYPE code) {
    struct input_event ev;
    memset(&ev, 0, sizeof(struct input_event));
    ev.type = EV_KEY;
    ev.code = code;
    ev.value = 0;

    if(write(fd, &ev, sizeof(struct input_event)) < 0) {
        printf("error: sync-report");
        return ;
    }
    send_syn();
}

void move_cursor(int x, int y) {
    struct input_event ev[2];
    memset(&ev, 0, sizeof(ev));
    ev[0].type = EV_ABS;
    ev[0].code = ABS_X;
    ev[0].value = x;
    ev[1].type = EV_ABS;
    ev[1].code = ABS_Y;
    ev[1].value = y ;

    if(write(fd, ev, sizeof(ev)) < 0)
    {
        printf("error: ABS_Y-write");
        return ;
    }
    send_syn();
}

void input_click(int x, int y, __U16_TYPE code) {
    struct input_event ev[3];
    memset(&ev, 0, sizeof(ev));
    ev[0].type = EV_ABS;
    ev[0].code = ABS_X;
    ev[0].value = x;
    ev[1].type = EV_ABS;
    ev[1].code = ABS_Y;
    ev[1].value = y ;
    ev[2].type = EV_KEY;
    ev[2].code = code;
    ev[2].value = 1;
    if(write(fd, ev, sizeof(ev)) < 0) {
        printf("error: ABS_Y-write");
        return ;
    }
    send_syn();
}

void input_click_up(int x, int y, __U16_TYPE code) {
    struct input_event ev[4];
    memset(&ev, 0, sizeof(ev));
    ev[0].type = EV_ABS;
    ev[0].code = ABS_X;
    ev[0].value = x;
    ev[1].type = EV_ABS;
    ev[1].code = ABS_Y;
    ev[1].value = y ;
    ev[2].type = EV_KEY;
    ev[2].code = code;
    ev[2].value = 1;
    ev[3].type = EV_KEY;
    ev[3].code = code;
    ev[3].value = 0;
    if(write(fd, ev, sizeof(ev)) < 0) {
        printf("error: ABS_Y-write");
        return ;
    }
    send_syn();
}

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

int main(int argc, char **argv)
{
    QCoreApplication a(argc, argv);
    if(init() == 1)
        return 1;

//    input_click_up(300, 300, BTN_LEFT);
//    release_button(BTN_LEFT);
//    usleep(_wait);

//    move_cursor(300, 400);
//    usleep(_wait);

//    input_click(300, 500, BTN_LEFT);
//    release_button(BTN_LEFT);
//    usleep(_wait);



//    input_click(300, 300, BTN_LEFT);
//    release_button(BTN_LEFT);
//    usleep(_wait);

//    move_cursor(300, 400);
//    usleep(_wait);

//    input_click(300, 500, BTN_LEFT);
//    release_button(BTN_LEFT);
//    usleep(_wait);

//    start_server();
    TcpServer server;



    return a.exec();
}
