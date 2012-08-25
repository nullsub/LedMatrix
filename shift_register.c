#include "shift_register.h"

void shift_init()
{		
	// define shift-register pins as output
	SHIFT_DDR |= (1<<SHIFT_RCK) | (1<<SHIFT_SCK)| (1<<SHIFT_SER)|(1<<SHIFT_NOT_ENABLE);
	SHIFT_PORT &= ~((1<<SHIFT_RCK) | (1<<SHIFT_SCK) | (1<<SHIFT_SER)|(1<<SHIFT_NOT_ENABLE));

	shift_output_disable();
	return;
}

void shift_out()
{
	unsigned char out_byte;

	for(int register_cnt = 0; register_cnt < NR_OF_REGISTERS; register_cnt++) {
		out_byte = register_state[(NR_OF_REGISTERS-1)-register_cnt];
		for(int bit_counter = 0; bit_counter < BITS_TO_SHIFT; bit_counter ++) {
			if(out_byte & 0b10000000){
				SHIFT_PORT |= (1<<SHIFT_SER);  
			}
			else{
				SHIFT_PORT &= ~(1<<SHIFT_SER);
			}
			out_byte  = (out_byte  <<  1); 

			SHIFT_PORT |= (1<<SHIFT_SCK); 
			//__asm("nop"); //works without aswell
			SHIFT_PORT &= ~(1<<SHIFT_SCK);;
		}	
	}

	//Latch-->output
	SHIFT_PORT |= (1<<SHIFT_RCK); 

	SHIFT_PORT &= ~(1<<SHIFT_NOT_ENABLE); //delay needed anyway

	SHIFT_PORT &= ~(1<<SHIFT_RCK); 

	return;
}

void shift_output_disable()
{
	SHIFT_DDR  |= (1<<SHIFT_NOT_ENABLE);
	SHIFT_PORT |= (1<<SHIFT_NOT_ENABLE);
}

