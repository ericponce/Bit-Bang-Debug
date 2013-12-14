#include <avr/io.h>
#include <util/delay.h>
#include "uart_drv.h"



int main(void) {
	volatile char inputBuffer[8];

	char foo = 'a';
	uart_open(9600, inputBuffer, 8);
	sei();

	while(1) {
		foo = inputBuffer[0];
		if (foo != 0x00){
			uart_write(&foo, 1);
		}
		_delay_ms(50);
	}

	uart_close();

	return 1;
}