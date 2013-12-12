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
} State;

volatile State state;

ISR(TIM1_COMPA_vect) {
	// switch (state.state) {
	// 	case IDLE:
	// 		break;
	// 	case TRANSMIT:
	// 		if (state.count == 9) {
	// 			uart_set_low();
	// 		}
	// 		else if (state.count > 0) {
	// 			if (*state.data & 1) {
	// 				uart_set_low();
	// 			} else {
	// 				uart_set_high();
	// 			}
	// 			*state.data = *state.data >> 1;
	// 		} else if ( state.count == 0) {
	// 			uart_set_high();
	// 			state.state = TRANSMIT_STOP_BIT;
	// 		}
	// 		state.count--;
	// 		break;
	// 	case TRANSMIT_STOP_BIT:
	// 		uart_stop_timing();
	// 		uart_enable_read();
	// 		state.state = IDLE;
	// 		break;
	// }

	if (state.count == 9) {
		uart_set_low();
	}

	else if (state.count > 0 ){
		if (state.data & 1) {
				uart_set_high();
		} else {
				uart_set_low();
		}
		state.data = state.data >> 1;
	}

	else {
		uart_set_high();
		state.state = IDLE;
		uart_stop_timing();
	}

	state.count--;
}

// ISR(INT0_vect) {
// 	state.state = RECEIVE;
// 	state.count = 0;
// 	uart_set_baud(state.baud * 1.5);
// 	uart_start_timing();

// }

/**
* Sets up UART Com
*
* returns != 0 if good, 0 if bad
*
*/
uint8_t uart_open(unsigned long b_rate){

	//Clear timer on compare match with OCR1C
	uart_set_up();
	//uart_enable_read();
	//When to "overflow"
	if (b_rate == 9600){
		state.baud = 104;
	}
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
	//uart_disable_read();
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

char uart_read(void){
	// if (state.state == DATA_PENDING) {
	// 	state.state = IDLE;
	// 	return state.data;
	// } else {
	// 	return 0;
	// }
	return 1;
}

void uart_ioctl(void){}