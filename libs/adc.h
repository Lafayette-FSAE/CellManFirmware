#ifndef __ADC
#define __ADC

#include "stm8.h"

unsigned int adc_read(unsigned int channel){
	
	unsigned int output = 0;

	ADC_CSR &= 0b01111111;

	ADC_CSR |= ((0x0F) & channel); // Select Channel
	ADC_CR2 |= (1 << 3); // Right Aligned DATA
	ADC_CR1 |= (1 << 0); // ADC ON
	ADC_CR1 |= (1 << 0); // ADC Start Conversion

	// Wait until EOC bit is set by hardware
	while((ADC_CSR) & 0b10000000 == 0);

	// copy data from data registers into value
	output |= (unsigned int) ADC_DRL;
	output |= (unsigned int) ADC_DRH << 8;

	// stop ADC conversion
	ADC_CR1 &= ~(1 << 0);

	output &= 0x3FF;
	return output;

}

#endif // __ADC