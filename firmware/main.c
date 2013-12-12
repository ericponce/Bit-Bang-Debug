#include <avr/io.h>
#include <util/delay.h>
#include "uart_drv.h"

int main(void) {
	DDRB |= 1 << PB1;
	char *message = "Hello!";

	uart_open(9600);
	sei();

	while(1) {
		uart_write(message, 7);
		_delay_ms(10);
	}

	uart_close();

	return 1;
}