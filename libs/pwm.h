#ifndef __PWM
#define __PWM

#include "stm8.h"
#include "utils.h"

int led_value = 0;

void pwm_inter (void) __interrupt 13 {
	// led_value = !led_value;
	// set_led(led_value);

	TIM2_SR1 &= ~(1 << 0);
}

void init_timer2(){
	TIM2_CR1 = 0;               // Turn everything TIM2 related off.
	TIM2_IER = 0;
	TIM2_SR2 = 0;
	TIM2_CCER1 = 0;
	TIM2_CCER2 = 0;
	TIM2_CCER1 = 0;
	TIM2_CCER2 = 0;
	TIM2_CCMR1 = 0;
	TIM2_CCMR2 = 0;
	TIM2_CCMR3 = 0;
	TIM2_CNTRH = 0;
	TIM2_CNTRL = 0;
	TIM2_PSCR = 0;
	TIM2_ARRH  = 0;
	TIM2_ARRL  = 0;
	TIM2_CCR1H = 0;
	TIM2_CCR1L = 0;
	TIM2_CCR2H = 0;
	TIM2_CCR2L = 0;
	TIM2_CCR3H = 0;
	TIM2_CCR3L = 0;
	TIM2_SR1 = 0;
}


void pwm_init(){
	init_timer2();

	TIM2_PSCR = 0x03;       //  Prescaler = 1.
	TIM2_ARRH = 0xFF;       //  High byte of 50,000.
	TIM2_ARRL = 0xFF;       //  Low byte of 50,000.
	TIM2_CCR1H = 0x80;      //  High byte of 12,500
	TIM2_CCR1L = 0xF0;      //  Low byte of 12,500

	TIM2_CCER1 &= ~(1 << 1);
	TIM2_CCER1 |= (1 << 0);

	TIM2_CCMR1 &= 0b11101111;
	TIM2_CCMR1 |= 0b01100000;

	// TIM2_CCER3 &= ~(1 << 1);
	// TIM2_CCER3 |= (1 << 0);

	// TIM2_CCMR3 &= 0b11101111;
	// TIM2_CCMR3 |= 0b01100000;

	TIM2_CCMR1 &= ~(1 << 4);
	TIM2_CCMR1 |= (3 << 5);

	TIM2_IER |= (1 << 0);
	TIM2_CR1 |= (1 << 0);
}

#endif // __PWM