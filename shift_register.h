#ifndef SHIFT_REGISTER_H
#define SHIFT_REGISTER_H SHIFT_REGISTER_H

#include <avr/io.h>
#include <inttypes.h>

#define SHIFT_RCK 	6
#define SHIFT_SCK       7
//#define SHIFT_SCL	2 no reset needed. is directly connected to VCC
#define SHIFT_SER     	5
#define SHIFT_NOT_ENABLE 4 
#define SHIFT_DDR 	DDRD
#define SHIFT_PORT 	PORTD
 
#define BITS_TO_SHIFT 8 // how many shift registers are connected
#define NR_OF_REGISTERS 4 // how many registers are in a row

int8_t register_state[NR_OF_REGISTERS];

void shift_init();
void shift_out();
void shift_output_disable();


#endif //SHIFT_REGISTER_H
