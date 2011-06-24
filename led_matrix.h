#ifndef LED_MATRIX_H
#define LED_MATRIX_H

#include <inttypes.h>

#define SIZE_X 16
#define SIZE_Y 16

#define COLLUMN_REG1 0  //shift register index
#define COLLUMN_REG2 1
#define LINE_REG1    2	
#define LINE_REG2    3

void led_matrix_init();
void led_matrix_set_pixel(uint8_t x, uint8_t y, uint8_t val);
void led_matrix_set_pixel_fb(uint8_t x, uint8_t y, uint8_t val, int8_t * custom_fb);
uint8_t led_matrix_get_pixel(uint8_t x, uint8_t y);
uint8_t led_matrix_get_pixel_fb(uint8_t x, uint8_t y, int8_t * custom_fb);
;
int8_t * led_matrix_set_fb(int8_t *new_fb); //returns old_fb
void reset_fb();

#endif
