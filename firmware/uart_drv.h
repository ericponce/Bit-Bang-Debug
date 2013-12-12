#include <avr/io.h>
#include <avr/interrupt.h>

#define UART_PORT_IN PORTB
#define UART_PORT_OUT PORTB
#define UART_PIN_IN PB0
#define UART_PIN_OUT PB1

//#define uart_set_up() {TCCR1 |= (1 << CTC1); MCUCR |= (1 << ISC01);} //Clear on overflow, external interupt on falling edge
#define uart_set_up() TCCR1 |= ((1 << CTC1) | (1 << CS10)) //Clear on overflow, external interupt on falling edge

#define uart_tear_down() (TCCR1 &= ~(1 << CTC1))
#define uart_set_baud(bd) {OCR1C = bd; OCR1A = bd;}
#define uart_reset_baud() (OCR1C = 0)

#define uart_start_timing() (TIMSK |= (1 << OCIE1A)) //Enable TImer 1 Interrupt
#define uart_stop_timing() (TIMSK &= ~(1 << OCIE1A))
#define uart_reset_timing() (TCNT1 = 0)

#define uart_set_high() (UART_PORT_OUT |= (1 << UART_PIN_OUT))
#define uart_set_low() (UART_PORT_OUT &= ~(1 << UART_PIN_OUT))
#define uart_get() ((UART_PIN_IN & (1 << UART_PIN_IN)) >> UART_PIN_IN)

#define uart_enable_read() (GIMSK |= (1 << INT0)) //Enable external interrupts
#define uart_disable_read() (GIMSK &= ~(1 << INT0))

extern uint8_t uart_open(unsigned long b_rate);
extern void uart_close(void);
extern char uart_read(void);
extern uint8_t uart_write(char *array, uint8_t len);
extern void uart_ioctl(void);