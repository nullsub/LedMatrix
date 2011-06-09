#ifndef LED_MATRIX_H
#define LED_MATRIX_H

#include <inttypes.h>

#define SIZE_X 16
#define SIZE_Y 16

#define COLLUMN_REG1 0  //shift register index
#define COLLUMN_REG2 1
#define LINE_REG1    2	
#define LINE_REG2    3

int8_t led_matrix_framebffr[(SIZE_X * SIZE_Y)/8];

void led_matrix_init();
void led_matrix_set_pixel(int8_t x,int8_t y,int8_t val);

#endif
