#include <stdio.h>
//#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
//#include <errno.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include <assert.h>
#include <X11/Xlib.h>


const int _wait = 500000;


static int fd;

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

void release_button(__U16_TYPE code = BTN_LEFT) {
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

void input_click(int x, int y, __U16_TYPE code = BTN_LEFT) {
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
//    ev[3].type = EV_KEY;
//    ev[3].code = code;
//    ev[3].value = 0;
//    ev[3].type = EV_KEY;
//    ev[3].code = BTN_LEFT;
//    ev[3].value = 0;
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

    uidev.absmin[ABS_X] = 0;
    uidev.absmax[ABS_X] = 1920 * 2; //Parameters of thumbsticks

    uidev.absmin[ABS_Y] = 0;
    uidev.absmax[ABS_Y] = 1080 + 1200;


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

int main()
{
    if(init() == 1)
        return 1;

    input_click(2500, 1500, BTN_LEFT);
    usleep(_wait);
    move_cursor(2500, 1600);
    usleep(_wait);
    release_button(BTN_LEFT);
    usleep(_wait);
    move_cursor(2500, 1500);



    printf("************");
    destroy_app();
    return 0;
}
