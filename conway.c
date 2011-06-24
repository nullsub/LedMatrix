#include "apps.h"

static int8_t * old_fb;
static int8_t * new_fb; 

uint8_t is_alive(uint8_t x, uint8_t y)
{
	return led_matrix_get_pixel( x, y);
}

#define is_dead(x,y)	(!(is_alive((x),(y))))

static uint8_t living_neighbors(uint8_t x, uint8_t y)
{
	uint8_t neighbors = 0;
	if(y > 0) { //above
		//middle
		if(is_alive(x,y-1)) {
			neighbors ++;
		} 
		//left
		if(x > 0) {
			if(is_alive(x-1,y-1)) {
				neighbors ++;
			}
		} 
		//right
		if(x < SIZE_X-1) {
			if(is_alive(x+1,y-1)) {
				neighbors ++;
			}
		} 
	}
	if(y < SIZE_Y-1) { //beneath
		//middle
		if(is_alive(x,y+1)) {
			neighbors ++;
		} 
		//left
		if(x > 0) {
			if(is_alive(x-1,y+1)) {
				neighbors ++;
			}
		} 
		//right
		if(x < SIZE_X-1) {
			if(is_alive(x+1,y+1)) {
				neighbors ++;
			}
		}
	} 
	//left
	if(x > 0) {
		if(is_alive(x-1,y)) {
			neighbors ++;
		}
	} 
	//right
	if(x < SIZE_X-1) {
		if(is_alive(x+1,y)) {
			neighbors ++;
		}
	}
	
	if(neighbors > 7) {
		uart_puts("neighbors > 7\n");
	}
	return neighbors;
}




void set_alive(uint8_t x, uint8_t y)
{
	new_fb[((SIZE_Y*y)+x)/8] |= (1<<(x%8)); // set
}

void set_dead(uint8_t x, uint8_t y)
{
	new_fb[((SIZE_Y*y)+x)/8] &= ~(1<<(x%8)); //clear
}

void conway_run(int8_t *start_pic)
{
	old_fb = start_pic;
	new_fb = led_matrix_set_fb(old_fb);
	uint8_t	changed = 1; // first time anyway

	for(int i = 0; i < SIZE_X; i++){
		led_matrix_set_pixel(i,i,1);
	}
	for(int i = 0; i < 100; i++)
		_delay_ms(20);

	app_reset_tick();
	app_start_tick();

	while(1) {
	if(old_fb == new_fb) {
		uart_puts("the same!!!\n");
		//malloc new fb
	} 

		for(uint8_t x = 0; x < SIZE_X; x ++) {
			for(uint8_t y = 0; y < SIZE_Y; y ++) {
				uint8_t neighbors = living_neighbors(x,y);
				if(neighbors == 3) {
					uart_puts("has 3 neighbors\n");
					if(is_dead(x,y)) {
						changed = 1;
					}
					set_alive(x,y);
				}else if(neighbors == 2) {
					if(is_alive(x,y)) {
						uart_puts("has_2 neighbors and is alive\n");
						set_alive(x,y);
					}else{
						set_dead(x,y);
					}
				}else {
					
					if(is_alive(x,y)) {
						changed = 1;
						if(neighbors == 1) {
							uart_puts("neighbors = 1, is alive\n");
						}else if(neighbors == 0){
							uart_puts("not 1 and not 0 but alive");
						}
					}
					set_dead(x,y);
				}
			}
		}
		if(changed == 0) {
			reset_fb();
			app_stop_tick();
			app_reset_tick();
			return;	
		}
		changed = 0;
		/*	while(app_get_tick()) {
			uart_puts("waiting\n");
			_delay_ms(10);
			}*/
		for(int k = 0; k < 100; k++) {
			_delay_ms(10);
			asm volatile("nop");
		}
		app_dec_tick();
		old_fb = new_fb;
		new_fb = led_matrix_set_fb(new_fb);
	}
}
