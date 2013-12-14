/**************************************
*									  *
*									  *
*		Uses Timer 1 				  *
*									  *
*									  *
**************************************/

#include <avr/io.h>
#include <avr/interrupt.h>
#include "uart_drv.h"

typedef enum{
    IDLE,
    TRANSMIT,
    TRANSMIT_STOP_BIT,
    RECEIVE,
    DATA_PENDING
 } UState;

typedef struct State {
	UState state;
	uint8_t baud;
	char data;
	uint8_t count;
	char *buffer;
	uint16_t read;
	uint16_t write;
	uint16_t bufferLength;
} State;

volatile State state;

//perhaps use pointers
uint8_t incrementIndex(uint16_t index, uint16_t len) {
	if (index + 1 == len) {
		return 0;
	} else {
		return index + 1;
	}
}

ISR(TIM1_COMPA_vect) {
	switch (state.state) {
		case IDLE:
			break;
		case TRANSMIT:
			if (state.count == 9) {
				uart_set_low();
			}else if (state.count > 0) {		
				if (state.data & 1) {
						uart_set_low();
				} else {
						uart_set_high();
				}
				state.data = state.data >> 1;
			} else {
				uart_set_high();
				state.state = IDLE;
				uart_stop_timing();
			}
			state.count--;
			break;
		// case TRANSMIT_STOP_BIT:
		// 	uart_stop_timing();
		// 	uart_enable_read();
		// 	state.state = IDLE;
		// 	break;
		// case RECEIVE:
		// 	if (state.count == 0) {
		// 		uart_set_baud(state.baud);
		// 	}
		// 	if (state.count < 8) {
		// 		state.buffer[state.write] = state.buffer[state.write] >> 1;
		// 		if (!uart_get()) {
		// 			state.buffer[state.write] |= 0x80;
		// 		}
		// 	} else {
		// 		state.state = DATA_PENDING;
		// 	}
		// 	state.count++;
		// 	break;
		// case DATA_PENDING:
		// 	state.state = IDLE;
		// 	uart_stop_timing();

		// 	//Optimize this shitty process
		// 	state.write = incrementIndex(state.write, state.bufferLength);
		// 	if (state.read == state.write) {
		// 		state.write = incrementIndex(state.read, state.bufferLength);
		// 	}
		// 	uart_enable_read();
		// 	break;
	}
}

// ISR(PCINT0_vect) {
// 	uart_disable_read();
// 	state.state = RECEIVE;
// 	state.count = 0;
// 	uart_set_baud(156);
// 	uart_set_timing(26);
// 	uart_start_timing();
// }

/**
* Sets up UART Com
*
* returns != 0 if good, 0 if bad
*
*/
uint8_t uart_open(unsigned long b_rate, char* readBuffer, uint16_t bufferLength){

	//Clear timer on compare match with OCR1C
	uart_set_up();
	uart_enable_read();
	//When to "overflow"
	if (b_rate == 9600){
		state.baud = 104;
	}

	state.buffer = readBuffer;
	state.read = 0;
	state.write = 0;
	state.bufferLength = bufferLength;

	state.state = IDLE;
	uart_set_high();
	return 1;
}

void uart_close(void){
	uart_tear_down();
	uart_reset_baud();
}

uint8_t _put_char(char *byte) {
	while(state.state != IDLE) {

	}
	uart_disable_read();
	state.state = TRANSMIT;
	state.data = *byte;
	state.count = 9;
	uart_set_baud(state.baud); //Make more standard
	uart_reset_timing();
	uart_start_timing();
	return 1;

}

uint8_t uart_write(char *array, uint8_t len){
	uint8_t i = 0;
	for (i = 0; i < len; i++) {
		_put_char(&array[i]);
	}
	return 1;
}

//THIS IS SHIT, FIX THIS
char uart_read(void){
	// char c;
	// if (state.read != state.write) {
	// 	c = state.buffer[state.read];
	// 	state.read = incrementIndex(state.read, state.bufferLength);
	// } else {
	// 	c = 0;
	// }
	// return c;
	return 0;
}

void uart_ioctl(void){}