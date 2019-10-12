#ifndef MAIN_H
#define MAIN_H
//#include <QDesktopWidget>
#include <QCoreApplication>
#include <QApplication>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include "tcpserver.h"



void send_syn();
void destroy_app();

void push_shift_key_button(__U16_TYPE code);
void push_key_button(__U16_TYPE code);
void press_button_key(__U16_TYPE code);
void release_button_key(__U16_TYPE code);
void mouse_click(__U16_TYPE code);
void mouse_move_click(int x, int y, __U16_TYPE code);
void mouse_move_press(int x, int y, __U16_TYPE code);
void mouse_move_release(int x, int y, __U16_TYPE code);
void mouse_move(int x, int y);

const int _wait = 500000;

#endif // MAIN_H

// bla bla
