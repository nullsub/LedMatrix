#include <stdio.h>
#include <errno.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <poll.h>
#include <pulse/pulseaudio.h>

#include "output.h"
#include "uart_communication.h"

/*
static void imon_handle_events(pa_mainloop_api *api, pa_io_event *e,
        int fd, pa_io_event_flags_t events, void *nill)
{
	return;
    struct timeval tv;
    memset(&tv, 0, sizeof(tv));
    libusb_handle_events_timeout(context, &tv);
}
*/
int imon_open()
{
	port_open();
}

void imon_close(pa_mainloop_api *api)
{
	port_close();	
}

void imon_write(const char *data) //Expects an array with 16 integers specifing the levels
{
	char str[30];
	for(int j = 0; j < 16; j++) {
		for(int i = 0; i < data[j]; i++) {
			printf("#");
			sprintf(str, "set %i %i\n",j , i);
			//printf("%s", str);
			port_write(str, strlen(str));
		}
		printf("\n");
	}
	return;
}
