#ifndef __PWM
#define __PWM

#include "stm8.h"
#include "utils.h"

// the period of the pwm signal in units of clock cycles
uint16_t cycles;
void pwm_set_freq(float freq){
	// assume prescaler of 1,
	// should mean a clock speed of 16MHz

	// expect freq as a float with units of kHz
	float period = 1 / freq;

	// convert period from units in seconds to an integer multiple
	// of clock cycles

	cycles = (uint16_t)(period * 16000000);
	TIM2_ARRH = msb(cycles);
	TIM2_ARRL = lsb(cycles);
}

void pwm_set_duty(uint16_t duty_cycle_percent){
	float duty = (float)(duty_cycle_percent) / 100;

	uint16_t cycles_on = (uint16_t)(cycles * duty);

	TIM2_CCR1H = msb(cycles_on);
	TIM2_CCR1L = lsb(cycles_on);
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

	TIM2_PSCR = 0x01;       //  Prescaler = 1.

	pwm_set_freq(10.0); // 10kHz
	pwm_set_duty(20); 	// 20 percent

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

	// TIM2_IER |= (1 << 0);
	TIM2_CR1 |= (1 << 0);
}

#endif // __PWM