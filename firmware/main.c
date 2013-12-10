#include <avr/io.h>
#include "uart_drv.h"

int main(void) {

	char *message = "Test";

	uart_open(9600);
	uart_write(message, 6);
	uart_close();

	return 1;
}