#include <stdio.h>
#include <errno.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <poll.h>
#include <pulse/pulseaudio.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

#include "output.h"
#include "uart_communication.h"

void error(char *str) 
{
	printf("%s", str);
	exit(-1);
}    	

int sockfd;

int imon_open()
{	
	int portno;
	struct sockaddr_in serv_addr;
	struct hostent *server;

	portno = 2000;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) 
		error("ERROR opening socket");
	server = gethostbyname("192.168.2.12");
	if (server == NULL) {
		fprintf(stderr,"ERROR, no such host\n");
		exit(0);
	}
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr, 
			(char *)&serv_addr.sin_addr.s_addr,
			server->h_length);
	serv_addr.sin_port = htons(portno);
	if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
		error("ERROR connecting");
	return 1;
	//	return port_open();
}

void imon_close()
{
	close(sockfd);
	//port_close();	
}

void write_to_device(char * str)
{
	//printf("%s", str);
	write(sockfd,str,strlen(str));
}

void imon_write(const char *data) //Expects an array with 16 integers specifing the levels
{
	char str[30];
	str[0] = 'm';
	str[1] = 0x00;	
	write_to_device(str);
	for(int j = 0; j < 16; j++) {
		sprintf(str, " %i", data[j]);
		write_to_device(str);
		//port_write(str, strlen(str));
	}
	str[0] = '\n';
	str[1] = 0x00;	
	write_to_device(str);
	return;
}

