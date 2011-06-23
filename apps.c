#include "apps.h"

static volatile tick;

void apps_init()
{
 	// config the timer every 100ms
	TCCR2 |= (1<<WGM21)|(1<<CS22)|(CS21)|(1<<CS20);	 //CTC, prescaler 1024
	OCR2 = 123; // ((0.1/(1/F_CPU))/PRESCALER)-1
	TIMSK |= (1<<OCIE2); 
	
}

void app_start_tick()
{
	
}

void app_stop_tick()
{

}

uint8_t apps_get_tick()
{
	cli();
	uint8_t tmp = tick;
	sei();
	return tmp;
}

