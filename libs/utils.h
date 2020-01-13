#ifndef __UTILS
#define __UTILS

#include "stm8.h"

#define LED_PORT    PA
#define LED_PIN     PIN3

void set_led(int value){
	if(value){
		PORT(LED_PORT, ODR) |= LED_PIN; // PB_ODR |= (1 << 5);
	} else {
		PORT(LED_PORT, ODR) &= ~LED_PIN; // PB_ODR &= ~(1 << 5);
	}
}

/* Simple busy loop delay */
void delay(unsigned long count) {
	while (count--) nop();
}

#endif