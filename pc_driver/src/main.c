/*
 *
 *+  Copyright (c) 2009 Ian Halpern
 *@  http://impulse.ian-halpern.com
 *
 *   This file is part of Impulse.
 *
 *   Impulse is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   Impulse is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with Impulse.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "Impulse.h"
#include <stdio.h>
#include <uart_communication.h>

void led_display(char *data) //Expects an array with 16 integers specifing the levels
{
	char str[30];
	str[0] = 'm';
	str[1] = 0x00;	
	port_write(str, strlen(str));
	for(int j = 0; j < 16; j++) {
		if(data[j] > 16) data[j] = 16;
		sprintf(str, " %i", data[j]);
	}
	port_write(str, strlen(str));
	str[0] = '\n';
	str[1] = 0x00;	
	port_write(str, strlen(str));
	return;
}

int main( ) {

	port_open();
	im_start( );
	char crrnt_frame[16];

	while ( 1 ) {
		usleep( 1000000 / 30 );
		double *array = im_getSnapshot( IM_FFT );
		int i;
		for ( i = 0; i*16 < 256; i++) {
			crrnt_frame[i] = (19.0*array[i*16]);
			printf( " %i", crrnt_frame[i]);
		}
		printf( "\n" );
		fflush( stdout );
	}
	im_stop( );

	return 0;
}
