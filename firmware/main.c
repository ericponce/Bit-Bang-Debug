#include <avr/io.h>
#include <util/delay.h>
#include "uart_drv.h"

char inputBuffer[8];
uint8_t shift = 2;

// int main(void) {
// 	uart_set_up_test();

// 	uart_open(9600, inputBuffer, 8);
// 	sei();

// 	while(1) {
// 		uart_test_high();
// 		uart_write(0, 1);
// 		uart_test_low();
// 	}

// 	uart_close();

// 	return 1;
// }

int main(void) {
	uart_set_up_test();
	char foo = 'F';

	uart_open(9600, inputBuffer, 8);
	sei();

	while(1) {

		uart_write(&foo, 1);
		// foo = uart_read();

		// if (foo == 0){
		// 	uart_test_low();
		// } else {
		// 	uart_test_high();
		// }
	}

	uart_close();

	return 1;
}