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
 } UState;

typedef struct State {
	UState state;
	uint8_t baud;
	char data;
	uint8_t count;
	char *buffer;
	uint8_t read;
	uint8_t write;
	uint8_t len;
} State;

volatile State state;

uint16_t getNextIndex(uint8_t i, uint8_t len) {
	if (i + 1 == len) {
		return 0;
	} else {
		return i + 1;
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
						uart_set_high();
				} else {
						uart_set_low();
				}
				state.data = state.data >> 1;
			} else {
				uart_set_high();
				state.state = TRANSMIT_STOP_BIT;
			}
			state.count--;
			break;
		case TRANSMIT_STOP_BIT:
			uart_stop_timing();
			uart_enable_read();
			state.state = IDLE;
			break;
		case RECEIVE:

			if (state.count == 0) {
				uart_set_baud(state.baud);
			}
			if (state.count < 8) {
				state.data = state.data >> 1;
				state.data |= uart_get();
			} 
			else if (state.count == 8) {

			} else {
				state.buffer[state.write] = state.data;
				state.state = IDLE;
				state.write = getNextIndex(state.write, state.len);
				if (state.write == state.read) {
					state.read = getNextIndex(state.read, state.len);
				}
				uart_read_interrupt_reset();
				uart_enable_read();
			}
			state.count++;

			break;
	}
}

ISR(PCINT0_vect) {
	uart_disable_read();
	state.state = RECEIVE;
	state.count = 0;
	uart_set_baud(156-70);
	uart_reset_timing();
	uart_start_timing();
}

/**
* Sets up UART Com
*
* returns != 0 if good, 0 if bad
*
*/
uint8_t uart_open(unsigned long b_rate, char* readBuffer, uint8_t bufferLength){
	//Clear timer on compare match with OCR1C
	uart_set_up();
	DDRB |= 1 << TEST_PIN;

	uart_enable_read();
	//When to "overflow"
	if (b_rate == 9600){
		state.baud = 104;
	}

	state.buffer = readBuffer;
	state.read = 0;
	state.write = 0;
	state.len = bufferLength;

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

uint8_t uart_read(char * out){
	if (state.read == state.write) {
		return 0;
	} else {
		*out = state.buffer[state.read];
		state.read = getNextIndex(state.read, state.len);
	}
	return 1;
}

void uart_ioctl(void){}