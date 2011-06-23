
#include "uart.h"
#include "led_matrix.h"
#include "shift_register.h"
#include <stdlib.h>
#include <inttypes.h>

void parse_cmd(char *cmd);
int strcmp(char *str1, char *str2);
char *get_nxt_word(char *str, char* word);

#define MAX_CMD_LENGTH 40

int main(){
	char tmp;
	char cmd_bffr[MAX_CMD_LENGTH];
	int i = 0;
	uart_init();
	led_matrix_init();
	uart_puts("all initialized\n$");
	while(1){
		tmp = uart_getc();
		if(tmp != 0x00){
			uart_putc(tmp);
			if(tmp == '\b'){
				if(i > 0){
					i--;
					//uart_putc('\b');
				}
			}
			else if(tmp == '\n'|| tmp == '\r' ||
					 i >= MAX_CMD_LENGTH){
				if(i != 0){
					cmd_bffr[i] = 0x00;
					parse_cmd(cmd_bffr);
					i = 0;
					cmd_bffr[i] = 0x00;
				}
				uart_puts("\n$");
			}
			else{
				cmd_bffr[i] = tmp;
				i ++;
			}
		}
	}
	uart_puts("\nshouldn be here\n");
}



void parse_cmd(char *cmd)
{
	char command[MAX_CMD_LENGTH];
	char args[MAX_CMD_LENGTH];
	char an_arg[MAX_CMD_LENGTH];
	char * arg_pointer;
	args[0] = 0x00;
	command [0] = 0x00;
	int tmp, index;
	while(*cmd == ' '){
		cmd ++;
	}
	for(index = 0; *(cmd+sizeof(char)*index) != ' ' && 
		*(cmd+sizeof(char)*index) != 0x00 && 
			index < MAX_CMD_LENGTH ; index ++) {
		command[index] = *(cmd+sizeof(char)*index);
	}
	command[index] = 0x00;
	tmp = index;
	for(; *(cmd+sizeof(char)*index) != 0x00 && 
		index < MAX_CMD_LENGTH; index ++) {
		args[index-tmp] = *(cmd+sizeof(char)*index);
	}
	args[index-tmp] = 0x00;

	arg_pointer = get_nxt_word(args+1, an_arg);	

	if(!strcmp(command, "clearpixels")){
		for(unsigned int i = 0; i < SIZE_Y; i++) {
			for(unsigned int j = 0; j < SIZE_X; j++) {
				led_matrix_set_pixel(j,i,0);
			}
		}
		return;
	}if(!strcmp(command, "setpixels")){
		for(unsigned int i = 0; i < SIZE_Y; i++) {
			for(unsigned int j = 0; j < SIZE_X; j++) {
				led_matrix_set_pixel(j,i,1);
			}
		}
		return;
	}

	if(!strcmp(command, "set")){
		uart_puts("cmd set <line> <column>:\n");
		int line = atoi(an_arg);
		arg_pointer = get_nxt_word(arg_pointer, an_arg);
		int collumn = atoi(an_arg);
		register_state[0] = (int8_t) (line & 0xFF);
		register_state[1] = (int8_t)((line >> 8) & 0xFF);

		register_state[2] = (int8_t) (collumn & 0xFF);
		register_state[3] = (int8_t)((collumn >> 8) & 0xFF);
		shift_out();
		return;
	}
	if(!strcmp(command, "pong")){
		uart_puts("starting pong\n");
		pong_run();
		return;
	}if(!strcmp(command, "reset")){
		uart_puts("resetting...\n");
		__asm("rjmp 0x0000");
		return;
	}

	if(!strcmp(command, "help")){
		uart_puts("\navailable commands:"
			"\n\tsetpixels\t\tset all pixels"
			"\n\tpong\t\tstart the pong game"
			"\n\tclearpixels\t\tclear all pixels"
			"\n\treset\t\treset the controller");
		return;
	}
	uart_puts("unknown cmd\n");
}

char *get_nxt_word(char *str, char* word)
{
	int index;
	for(index = 0;*(str+sizeof(char)*index) != 0x00 && 
		*(str+sizeof(char)*index) != ' ' && 
			index < MAX_CMD_LENGTH; index += sizeof(char)) {
		word[index] = *(str+sizeof(char)*index);
	}
	word[index] = 0x00;
	return (char*)str+sizeof(char)*(index+1);
}

int strcmp(char *str1, char *str2)
{
	while(*str1 == *str2){		
		if(*str1 == 0x00) return 0;
		str1++;
		str2++;
	}
	return 1;
}

