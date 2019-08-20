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


//using namespace std;

static void uinput_postkey(int fd, unsigned key) {
    struct input_event ev;
    int rc;
    memset(&ev, 0, sizeof (ev));
    ev.type = EV_KEY;
    ev.code = key;
    ev.value = 1;
    rc = write(fd, &ev, sizeof (ev));
    assert(rc == sizeof (ev));
    ev.value = 0;
    rc = write(fd, &ev, sizeof (ev));
    assert(rc == sizeof (ev));
}

int main()
{
    int fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK); //opening of uinput
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

//    ioctl(fd, UI_SET_EVBIT, EV_SYN);





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

//    struct input_event syn;
    struct input_event ev[4];
    struct input_event evS;

    const int _wait = 350000;

    usleep(_wait);
//    sleep(1);
    int _i = 1;
    while(_i--)
    {
        printf("-------\n");
        memset(&ev, 0, sizeof(ev)); //setting the memory for event
        ev[0].type = EV_ABS;
        ev[0].code = ABS_X;
        ev[0].value = 2500;
        ev[1].type = EV_ABS;
        ev[1].code = ABS_Y;
        ev[1].value = 1500 ;
        ev[2].type = EV_KEY;
        ev[2].code = BTN_RIGHT;
        ev[2].value = 0;
        ev[3].type = EV_KEY;
        ev[3].code = BTN_RIGHT;
        ev[3].value = 1;        if(write(fd, ev, sizeof(ev)) < 0) //writing the thumbstick change
        {
            printf("error: ABS_Y-write");
            return 1;
        }


        memset(&evS, 0, sizeof(struct input_event));
        evS.type = EV_SYN;
        evS.code = SYN_REPORT;
        evS.value = 0;
        usleep(_wait);
        if(write(fd, &evS, sizeof(struct input_event)) < 0) //writing the sync report
        {
            printf("error: sync-report");
            return 1;
        }
//        uinput_postkey(fd, 0);

    }
    usleep(_wait);
    if(ioctl(fd, UI_DEV_DESTROY) < 0)
    {
        printf("error: ioctl");
        return 1;
    }
    close(fd);

    return 0;
}
