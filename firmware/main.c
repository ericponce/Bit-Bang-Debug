#include <avr/io.h>
#include <util/delay.h>
#include "uart_drv.h"



int main(void) {
	volatile char inputBuffer[8];

	char foo;
	uart_open(9600, inputBuffer, 8);
	sei();

	while(1) {
		if (uart_read(&foo)){
			uart_write(&foo,1);
		}
	}

	uart_close();

	return 1;
}