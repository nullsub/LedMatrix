/*
 * uart_communication.h
 *
 *  Created on: 10.07.2010
 *      Author: ch
 */

#ifndef UART_COMMUNICATION_H_
#define UART_COMMUNICATION_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <sys/ioctl.h>

int port_open(void);
int port_write(char * data, int size);
int port_read(char *bffr);
void port_close();

#endif /* UART_COMMUNICATION_H_ */
