
#include <avr/interrupt.h>
#include <avr/io.h>

#include "led_matrix.h"
#include "shift_register.h"

ISR (TIMER1_COMPA_vect)
{
	static int8_t crrnt_collumn = 0;
	register_state[LINE_REG1] = 
		led_matrix_framebffr[(crrnt_collumn*SIZE_Y)/8];
	register_state[LINE_REG2] = 
		led_matrix_framebffr[((crrnt_collumn*SIZE_Y)/8)+1];
	if(crrnt_collumn < 8){
		register_state[COLLUMN_REG1] = 
			~((int8_t)(0x01 << crrnt_collumn));
		register_state[COLLUMN_REG2] = ~0x00;
	}
	else{
		register_state[COLLUMN_REG2] = 
			~((int8_t)(0x01 << (crrnt_collumn-8)));
		register_state[COLLUMN_REG1] = ~0x00;	
	}
	shift_out();	
	crrnt_collumn ++;
	if(crrnt_collumn >= 16) 
		crrnt_collumn = 0;
//	__asm("wdr"); // reset watchdog
}


void led_matrix_init()
{
	shift_init();
	
	DDRD |= (1<<2);
	PORTD |= (1<<2);


 	 // CTC Modus
  	TCCR1B |= (1<<WGM12)|(1<<CS11)|(1<<CS10); // Prescaler 64
	OCR1A = 77;//((0.000625 / (1/F_CPU)) / 64)-1;   //  (TimeToGet/(1/F_CPU))/PRESCALER == to write in OCR0A
 	
  	// Compare Interrupt erlauben
  	TIMSK |= (1<<OCIE1A);
 
  /*	// Global Interrupts aktivieren
	WDTCR |= (1<<WDCE)|(1<<WDE);
	WDTCR |= (1<<WDP0)|(1<<WDP1)|(1<<WDE); //enable watchdog approximatily every 12ms
*/
  	sei();
}
 

void led_matrix_set_pixel(uint8_t x, uint8_t y, int8_t val)
{
	int8_t bit_pos = x%8;

	if(x >= SIZE_X || y >= SIZE_Y){
		return;
	}
	
	if(val == 1){
		led_matrix_framebffr[((SIZE_Y*y)+x)/8] |= (1<<bit_pos); // set
	}
	else{
		led_matrix_framebffr[((SIZE_Y*y)+x)/8] &= ~(1<<bit_pos); //clear
		
	} 	
}


