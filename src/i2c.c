#include <stdint.h>
#include "stm8.h"

// https://blog.mark-stevens.co.uk/2015/05/stm8s-i2c-slave-device/

/* I2C */
#define I2C_CR1 *(volatile unsigned char *)0x5210
#define I2C_CR2 *(volatile unsigned char *)0x5211
#define I2C_FREQR *(volatile unsigned char *)0x5212
#define I2C_OARL *(volatile unsigned char *)0x5213
#define I2C_OARH *(volatile unsigned char *)0x5214
#define I2C_DR *(volatile unsigned char *)0x5216
#define I2C_SR1 *(volatile unsigned char *)0x5217
#define I2C_SR2 *(volatile unsigned char *)0x5218
#define I2C_SR3 *(volatile unsigned char *)0x5219
#define I2C_ITR *(volatile unsigned char *)0x521A
#define I2C_CCRL *(volatile unsigned char *)0x521B
#define I2C_CCRH *(volatile unsigned char *)0x521C
#define I2C_TRISER *(volatile unsigned char *)0x521D
#define I2C_PECR *(volatile unsigned char *)0x521E

#define WRITE_I2C_CR1(val) ((*(volatile unsigned char *)0x5210) = (val))

void i2c_init(){
	rim(); // enable interrupts

	I2C_CR1 = 0;					// Disable I2C peripheral in order to modify it
	I2C_FREQR = 16;					//  Set the internal clock frequency (MHz).

	I2C_CCRH &= 0b01110000;
	I2C_CCRL = 0xa0;				//  SCL clock speed is 50 KHz.

	// I2C_CCRH_F_S = 0;				//  I2C running is standard mode.
	// I2C_CCRH_CCR = 0x00;

	// I2C_OARH_ADDMODE = 0;           //  7 bit address mode.
	// I2C_OARH_ADD = 0;               //  Set this device address to be 0x50.
	// I2C_OARL_ADD = 0x50;
	// I2C_OARH_ADDCONF = 1;           //  Docs say this must always be 1.

	// I2C_TRISER = 17;

	// I2C_ITR_ITBUFEN = 1;            //  Buffer interrupt enabled.
	// I2C_ITR_ITEVTEN = 1;            //  Event interrupt enabled.
	// I2C_ITR_ITERREN = 1;

	// I2C_CR1_PE = 1;					// re-enable peripheral

	// I2C_CR2_ACK = 1;
}

void TIM1_overflow_Handler() __interrupt(11){

}