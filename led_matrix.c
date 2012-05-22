#include <avr/interrupt.h>
#include <avr/io.h>

#include "led_matrix.h"
#include "shift_register.h"

static int8_t main_fb[(SIZE_X * SIZE_Y)/8];
static int8_t * crrnt_fb;

ISR (TIMER2_COMP_vect)
{
	static int8_t crrnt_collumn = 0;
	register_state[LINE_REG1] = 
		crrnt_fb[(crrnt_collumn*SIZE_Y)/8];
	register_state[LINE_REG2] = 
		crrnt_fb[((crrnt_collumn*SIZE_Y)/8)+1];
	if(crrnt_collumn < 8){
		register_state[COLLUMN_REG1] = ~(0x01 << crrnt_collumn);
		register_state[COLLUMN_REG2] = ~0x00;
	}
	else{
		register_state[COLLUMN_REG2] = 	~(0x01 << (crrnt_collumn-8));
		register_state[COLLUMN_REG1] = ~0x00;	
	}
	shift_out();	
	crrnt_collumn ++;
	if(crrnt_collumn >= 16) 
		crrnt_collumn = 0;
}


void led_matrix_init()
{
	shift_init();
	
	DDRD |= (1<<2);
	PORTD |= (1<<2);

	//Use  8 Bit Timer2
 	TCCR2 |= (1<<WGM21)|(1<<CS22);	 //CTC, prescaler 64
	OCR2 = 77;  // (NeededTime/(1/F_CPU))/PRESCALER 
	TIMSK |= (1<<OCIE2); 

	crrnt_fb = main_fb; 
  	sei();
}
 

void led_matrix_set_pixel(uint8_t x, uint8_t y, uint8_t val)
{
	led_matrix_set_pixel_fb(x, y, val, crrnt_fb);
}

void led_matrix_set_pixel_fb(uint8_t x, uint8_t y, uint8_t val, int8_t * custom_fb)
{
	int8_t xtmp = x%8;

	if(x >= SIZE_X || y >= SIZE_Y){
		return;
	}
 
// Workaround for hardware bug. I soldered it the wrong way...	
	if(y%2){
		y-=1;
	}else {
		y+=1;
	}

	if(x%2){
		xtmp-=1;
	}else {
		xtmp+=1;
	}
	
	if(val == 1){
		custom_fb[((SIZE_Y*y)+x)/8] |= (1<<xtmp); // set
	}
	else{
		custom_fb[((SIZE_Y*y)+x)/8] &= ~(1<<xtmp); //clear
		
	} 	

}


uint8_t led_matrix_get_pixel(uint8_t x, uint8_t y)
{
	return led_matrix_get_pixel_fb(x,y, crrnt_fb);
}

uint8_t led_matrix_get_pixel_fb(uint8_t x, uint8_t y, int8_t * custom_fb)
{
	uint8_t xtmp = x%8;

	if(x >= SIZE_X || y >= SIZE_Y){
		return 0;
	}

// Workaround for hardware bug. I soldered it the wrong way...	
	if(y%2){
		y-=1;
	}else {
		y+=1;
	}

	if(x%2){
		xtmp-=1;
	}else {
		xtmp+=1;
	}

	if(custom_fb[((SIZE_Y*y)+x)/8] & (1<<xtmp))
		return 1;
	return 0;
}


int8_t * led_matrix_set_fb(int8_t * new_fb)
{
	int8_t * ret = crrnt_fb;
	crrnt_fb = new_fb;
	return ret;
}


void reset_fb()
{
	crrnt_fb = main_fb; 
}

