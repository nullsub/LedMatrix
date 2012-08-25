#ifndef UART_H
#define UART_H

#define RECIEVE_BUFFER_SIZE 128

void uart_init();
unsigned char uart_getc();
void uart_putc(char data);
void uart_puts(char * data);
unsigned char uart_get_byte_block(void); // fetch charackter from rinbuffer

#ifndef F_CPU
#error "F_CPU is not defined"
#endif
 
#define BAUD 9600UL      
 
#define UBRR_VAL ((F_CPU+BAUD*8)/(BAUD*16)-1)   
#define BAUD_REAL (F_CPU/(16*(UBRR_VAL+1)))     
#define BAUD_ERROR ((BAUD_REAL*1000)/BAUD) 

#if ((BAUD_ERROR<990) || (BAUD_ERROR>1010))
  #error Systematischer Fehler der Baudrate grösser 1% und damit zu hoch! 
#endif

#endif // UART_H
