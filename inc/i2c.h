#ifndef __I2C
#define __I2C

#include <stdint.h>
#include "stm8.h"

#define TEST_I2C 1234

void i2c_init(){
	rim(); // enable interrupts

	I2C_CR1 = 0;					// Disable I2C peripheral in order to modify it
	I2C_FREQR = 16;					//  Set the internal clock frequency (MHz).

	// I2C_CCRH_F_S = 0;				//  I2C running is standard mode.
	// I2C_CCRH_CCR = 0x00;

	I2C_CCRH &= 0b01110000;
	I2C_CCRL = 0xa0;				//  SCL clock speed is 50 KHz.

	// I2C_OARH_ADDMODE = 0;           //  7 bit address mode.
	// I2C_OARH_ADD = 0;               //  Set this device address to be 0x50.
	// I2C_OARH_ADDCONF = 1;           //  Docs say this must always be 1.

	I2C_OARH &= 0b01111001;
	I2C_OARH |= 0b01000001;

	I2C_OARL = 0x60;					//  Set this device address to be 0x50.

	I2C_TRISER = 17;

	// I2C_ITR_ITBUFEN = 1;            //  Buffer interrupt enabled.
	// I2C_ITR_ITEVTEN = 1;            //  Event interrupt enabled.
	// I2C_ITR_ITERREN = 1;

	I2C_ITR &= 0b1111111;
	I2C_ITR |= 0b0000111;

	I2C_CR1 |= 0b00000001;					// re-enable peripheral

	I2C_CR2 |= 0b00000100;
}

#endif