#include <avr/io.h>
#include <util/delay.h>
#include "uart_drv.h"

char inputBuffer[8];

// int main(void) {
// 	DDRB |= 1 << PB0;

// 	uart_open(9600, inputBuffer, 8);
// 	sei();

// 	while(1) {
// 		PORTB |= (1 << PB0);
// 		uart_write(0xBB, 1);
// 		PORTB &= ~(1 << PB0);
// 	}

// 	uart_close();

// 	return 1;
// }

int main(void) {
	DDRB |= 1 << PB4;
	//PORTB |= (1 << PB4);
	char foo;

	uart_open(9600, inputBuffer, 8);
	sei();

	while(1) {
		foo = uart_read();
		if (foo != 0x00){
			uart_write(&foo, 1);
		}
	}

	uart_close();

	return 1;
}