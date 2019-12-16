/* The "Hello world!" of microcontrollers. Blink LED on/off */
#include <stdint.h>

#include "stm8.h"
#include "i2c.h"
#include "utils.h"

/* I2C */
// #define I2C_CR1 *(volatile unsigned char *)0x5210
// #define I2C_CR2 *(volatile unsigned char *)0x5211
// #define I2C_FREQR *(volatile unsigned char *)0x5212
// #define I2C_OARL *(volatile unsigned char *)0x5213
// #define I2C_OARH *(volatile unsigned char *)0x5214
// #define I2C_DR *(volatile unsigned char *)0x5216
// #define I2C_SR1 *(volatile unsigned char *)0x5217
// #define I2C_SR2 *(volatile unsigned char *)0x5218
// #define I2C_SR3 *(volatile unsigned char *)0x5219
// #define I2C_ITR *(volatile unsigned char *)0x521A
// #define I2C_CCRL *(volatile unsigned char *)0x521B
// #define I2C_CCRH *(volatile unsigned char *)0x521C
// #define I2C_TRISER *(volatile unsigned char *)0x521D
// #define I2C_PECR *(volatile unsigned char *)0x521E

int address_received_flag;
int i2c_activity;

unsigned char value;
unsigned char temp_value;

// typedef enum {
// 	WAITING,
// 	ADDING,
// 	SENDING
// } StateType;

// StateType state;


unsigned int adc_read(unsigned int channel){
	unsigned int val=0;

	set_led(0);

	ADC_CSR &= 0b01111111;

	ADC_CSR |= ((0x0F) & channel); // Select Channel
	ADC_CR2 |= (1 << 3); // Right Aligned DATA
	ADC_CR1 |= (1 << 0); // ADC ON
	ADC_CR1 |= (1 << 0); // ADC Start Conversion

	// Wait until EOC bit is set by hardware
	while((ADC_CSR) & 0b10000000 == 0);

	// copy data from data registers into value
	val |= (unsigned int) ADC_DRL;
	val |= (unsigned int) ADC_DRH << 8;

	// stop ADC conversion
	ADC_CR1 &= ~(1 << 0);

	set_led(1);

	val &= 0x3FF;
	return val;

}

void i2c_inter (void) __interrupt 19 {

	// Check Address Bit
	// Occurs at start of I2C packet
	if(I2C_SR1 & 0b00000010){

		// clear ADDR by reading SR1 followed by SR3
		I2C_SR1;
		I2C_SR3;

		return;
	}

	// Check RXNE bit
	// Occurs after each byte when there is data to read in the DR register
	if(I2C_SR1 & 0b01000000){

		// reading I2C_DR also resets the RXNE bit
		// so it is important that this gets read every time
		value = I2C_DR;

		return;
	}

	// Check TXE bit
	if(I2C_SR1 & 0b10000000){
		I2C_DR = temp_value;

		return;
	}

	// Check AF bit
	// should occur when master stops requesting data
	if(I2C_SR2 & 0b00000100){

		// clear AF bit by writing a 0 to it
		I2C_SR2 &= 0b11111011;

		return;
	}

	// Check STOPF bit
	// Occurs at the end of the I2C packet
	if(I2C_SR1 & 0b00010000){

		// clear STOPF by reading SR1 then writing CR2
		I2C_SR1;

		// I2C_CR2 |= 0b00000010;
		I2C_CR2 &= 0b11111101;
		return;
	}

	// getting here likely indicates an error
	// reset the BERR bit
	I2C_SR2 &= 0b11111110;

	return;
}

int main(void)
{
    /* Set clock to full speed (16 Mhz) */
    CLK_CKDIVR = 0;

    /* GPIO setup */
    // Set pin data direction as output
    PORT(LED_PORT, DDR)  |= LED_PIN; // i.e. PB_DDR |= (1 << 5);
    // Set pin as "Push-pull"
	PORT(LED_PORT, CR1)  |= LED_PIN; // i.e. PB_CR1 |= (1 << 5);

	i2c_init();
	set_led(1);

	temp_value = 100;

	while(1) {

		temp_value = adc_read(3);

		if(value == 'A'){
			// set_led(0);
		} else {
			// set_led(1);
		}

		delay(value * 3000L);
	}
}