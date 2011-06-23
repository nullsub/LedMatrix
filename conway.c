#include "apps.h"

// field int8_t led_matrix_framebffr[(SIZE_X * SIZE_Y)/8];
static int8_t ** old_fb;

static int living_neighbors(int index)
{
	int neighbors = 0;
	const int bit_pos = index % 8;
	if(index >= SIZE_X) { //above
		//middle
		if(*old_fb[(index - SIZE_X)/8] & (0x01 << bit_pos)) {
			neighbors ++;
		} 
		//left
		if(bit_pos > 1) {
			if(*old_fb[(index - SIZE_X)/8] & (0x01 << (bit_pos-1))) {
				neighbors ++;
			}
		} else if(index % 16) {
			if(*old_fb[((index - SIZE_X)/8)-1] &
					(0x01 << 7)) {
				neighbors ++;
			}
		}
		//right
		if(bit_pos < 6) {
			if(*old_fb[(index - SIZE_X)/8] & (0x01 << (bit_pos+1))) {
				neighbors ++;
			}
		} else if(index % 15) {
			if(*old_fb[((index - SIZE_X)/8)+1] &
					(0x01 << 0)) {
				neighbors ++;
			}
		}
	}
	if(index < SIZE_Y*SIZE_X-SIZE_X) { //beneath
		//middle
		if(*old_fb[(index + SIZE_X)/8] & (0x01 << (bit_pos))) {
			neighbors ++;
		} 
		//left
		if(bit_pos > 1) {
			if(*old_fb[(index + SIZE_X)/8] & (0x01 << (bit_pos-1))) {
				neighbors ++;
			}
		} else if(index % 16) {
			if(*old_fb[((index + SIZE_X)/8)-1] &
					(0x01 << 7)) {
				neighbors ++;
			}
		}
		//right
		if(bit_pos < 6) {
			if(*old_fb[(index + SIZE_X)/8] & (0x01 << (bit_pos+1))) {
				neighbors ++;
			}
		} else if(index % 15) {
			if(*old_fb[((index + SIZE_X)/8)+1] &
					(0x01 << 0)) {
				neighbors ++;
			}
		}
	}
	if(bit_pos > 1) { // left
		if(*old_fb[index/8] & (0x01 << (bit_pos-1))) {
			neighbors ++;
		}
	} else if(index % 16) {
		if(*old_fb[(index/8)-1] &
				(0x01 << 7)) {
			neighbors ++;
		}
	}
	if(bit_pos < 6) {	//right
		if(*old_fb[index/8] & (0x01 << (bit_pos+1))) {
			neighbors ++;
		}
	} else if(index % 15) {
		if(*old_fb[(index/8)+1] &
				(0x01 << 0)) {
			neighbors ++;
		}
	}
	return neighbors;
}

#define set_alive(index) (*new_fb[(index)] |= (1<<((index)%8)))
#define set_dead(index)	(*new_fb[(index)] &= ~(1<<((index)%8)))

void conway_run(int8_t *start_pic[])
{
	old_fb = start_pic;
	int8_t ** new_fb = led_matrix_framebffr;
	int8_t ** tmp;

	while(1) {
		for(int i = 0; i < SIZE_X*SIZE_Y; i++) {
			int neighbors = living_neighbors(i);
			switch (neighbors) {
				case 2:
					break;
				case 3:
					set_alive(i);
					break;
				case 0:
				case 1:
				default:
					set_dead(i);
			}					
		}
		while(app_tick == 0) {
			asm("nop");
		}
		cli();
		app_tick --;
		sei();		

		led_matrix_set_fb(new_fb);

		tmp = old_fb;
		old_fb = new_fb;
		new_fb = tmp;
	}
}
