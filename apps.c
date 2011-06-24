#include <avr/interrupt.h>
#include <avr/io.h>

#include "apps.h"

static volatile uint8_t tick; 

void app_init()
{
 	 // Use 16 Bit Timer1 to create a tick every 100ms

  	TCCR1B = (1<<WGM12); 

/*		((NeededTime/(1/F_CPU))/PRESCALER)-1 	*/
/*		((0.1/(1/F_CPU))/1024)-1 = 780 		*/
 	OCR1A = 780; //16bit register

	TCNT1 = 0; //16bit register
 	TIMSK |= (1<<OCIE1A); // enable INT

	/* The Timer still needs to be started! (app_start_tick())*/
	tick = 0;		
}

void app_start_tick()
{
  	TCCR1B |= ((1<<CS12)|(1<<CS10)); // Prescaler 1024
}

void app_stop_tick()
{
 	TIMSK &= ~(1<<OCIE1A); // disable
  	TCCR1B &= ~((1<<CS12)|(1<<CS10)); 	//stop counter
}

void app_dec_tick()
{
	cli();
	if(tick)
		tick --;
	sei();
}
void app_reset_tick() 
{
	cli();
	tick = 0;
	sei();	
}

uint8_t app_get_tick()
{
	cli();
	uint8_t tmp = tick;
	sei();
	return tmp;
}

ISR (TIMER1_COMPA_vect)
{
	tick ++;
}

