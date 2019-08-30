#ifndef MAIN_H
#define MAIN_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <linux/input.h>
#include <linux/uinput.h>

void send_syn();
void release_button(__U16_TYPE code = BTN_LEFT);
void move_cursor(int x, int y);
void input_click(int x, int y, __U16_TYPE code = BTN_LEFT);
void input_click_up(int x, int y, __U16_TYPE code);
void simple_click_up(__U16_TYPE code);

void press_key(__U16_TYPE code);
void press_shift_key(__U16_TYPE code);

void destroy_app();
const int _wait = 500000;

#endif // MAIN_H
