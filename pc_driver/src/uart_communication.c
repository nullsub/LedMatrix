#include "uart_communication.h"

int fd; /* File descriptor for the port */

// write to the port
int port_write(char * str, int size) {
	int len = strlen(str);
	int n = write(fd, str, len);
	if (n != len)
		return -1;
	return 0;
}

int port_read(char *bffr){
	*bffr = 0x00;
	*(bffr+ sizeof(char)) = 0x00;
	read(fd, bffr,1);
	if(*bffr != 0x00){
		return 1;	
	}
	return 0;
}

void port_close() {
	close(fd);
}
/*
 * 'open_port()' - Open serial port 1.
 *
 * Returns the file descriptor on success or -1 on error.
 */
int port_open(void) 
{
	struct termios toptions;

	fd = open("/dev/ttyS0", O_RDWR | O_NOCTTY | O_NDELAY);
	if (fd == -1) {
		perror("serial_init: Unable to open serial port\n");
		return -1;
	}

	if (tcgetattr(fd, &toptions) < 0) {
		perror("serial_init: Could not get term attributes\n");
		return -1;
	}
	int baud = 115200;
	speed_t baud_rate = baud;
	switch(baud) {
	case 4800: baud_rate=B4800; break;
	case 9600: baud_rate=B9600; break;
#ifdef B14400
	case 14400: baud_rate=B14400; break;
#endif
	case 19200: baud_rate=B19200; break;
#ifdef B28800
	case 28800: baud_rate=B28800; break;
#endif
	case 38400: baud_rate=B38400; break;
	case 57600: baud_rate=B57600; break;
	case 115200: baud_rate=B115200; break;
	}
	cfsetispeed(&toptions, baud_rate);
	cfsetospeed(&toptions, baud_rate);

	toptions.c_cflag &= ~PARENB;
	toptions.c_cflag &= ~CSTOPB;
	toptions.c_cflag &= ~CSIZE;
	toptions.c_cflag |= CS8;
	toptions.c_cflag &= ~CRTSCTS;
	toptions.c_cflag |= CREAD | CLOCAL;
	toptions.c_iflag &= ~(IXON | IXOFF | IXANY);
	toptions.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
	toptions.c_oflag &= ~OPOST;

	toptions.c_cc[VMIN]  = 0;
  	toptions.c_cc[VTIME] = 20;

	if (tcsetattr(fd, TCSANOW, &toptions) < 0) {
		perror("serial_init: Could not set term attributes\n");
		return -1;
	}
	return fd;
}

