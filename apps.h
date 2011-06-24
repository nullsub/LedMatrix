#ifndef APPS_H_
#define APPS_H_

#include <inttypes.h>
#include <util/delay.h>
#include <avr/io.h>
#include "led_matrix.h"
#include "uart.h"

//api functions
void app_init();
void app_start_tick();
void app_stop_tick();
void app_reset_tick();
uint8_t app_get_tick();
void app_dec_tick();

//application functions
void pong_run();
void go_run();

/*expects a (SIZE_X*SIZE_Y)/8 byte array of the start image */
void conway_run(int8_t *start_pic);

#endif
