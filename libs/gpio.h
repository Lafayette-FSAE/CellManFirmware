#ifndef __GPIO
#define __GPIO

#include <stdint.h>
#include "stm8.h"

typedef enum {
	PORTA,
	PORTB,
	PORTC,
	PORTD
} gpio_port;


void gpio_init_as_output(gpio_port port, int position){
	switch(port) {
		case PORTA:
			PA_DDR |= (1 << position);
			break;
		case PORTB:
			PB_DDR |= (1 << position);
			break;
		case PORTC:
			PC_DDR |= (1 << position);
			break;
		case PORTD:
			PD_DDR |= (1 << position);
			break;
	}
}

void gpio_init_as_input(gpio_port port, int position){
	switch(port) {
		case PORTA:
			PA_DDR &= ~(1 << position);
			break;
		case PORTB:
			PB_DDR &= ~(1 << position);
			break;
		case PORTC:
			PC_DDR &= ~(1 << position);
			break;
		case PORTD:
			PD_DDR &= ~(1 << position);
			break;
	}
}

void gpio_set_pull_up(gpio_port port, int position, int value){
	
	unsigned char byte = (1 << position);

	switch(port){

		case PORTA:
			value ? (PA_CR1 |= byte) : (PA_CR1 &= ~byte);
			break;

		case PORTB:
			value ? (PB_CR1 |= byte) : (PB_CR1 &= ~byte);
			break;

		case PORTC:
			value ? (PC_CR1 |= byte) : (PC_CR1 &= ~byte);
			break;

		case PORTD:
			value ? (PD_CR1 |= byte) : (PD_CR1 &= ~byte);
			break;
	}
}

int gpio_read_from(gpio_port port, int position){

	unsigned char output = 0;

	switch(port){

		case PORTA:
			output = PA_IDR & (1 << position);
			break;

		case PORTB:
			output = PB_IDR & (1 << position);
			break;

		case PORTC:
			output = PC_IDR & (1 << position);
			break;

		case PORTD:
			output = PD_IDR & (1 << position);
			break;
	}

	return output ? 1 : 0;
}

#endif //  __GPIO