#ifndef MAIN_H
#define MAIN_H
//#include <QDesktopWidget>
#include <QCoreApplication>
#include <QApplication>
#include <QDateTime>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include "tcpserver.h"

#define MOUSE_MOVE_WAIT         30
#define MOUSE_CLICK_WAIT        30

void send_syn();
void destroy_app();

void push_shift_key_button(__U16_TYPE code);
void push_button_key(__U16_TYPE code, uint anMSec = 30);
void press_button_key(__U16_TYPE code);
void release_button_key(__U16_TYPE code);
void mouse_click(__U16_TYPE code);
void mouse_wheel(__U16_TYPE code, int nWheel);
void mouse_move_click(int x, int y, __U16_TYPE code);
void mouse_move_press(int x, int y, __U16_TYPE code);
void mouse_move_release(int x, int y, __U16_TYPE code);
void mouse_move(int x, int y, QString rel = "");

const int _wait = 500000;

#endif // MAIN_H
