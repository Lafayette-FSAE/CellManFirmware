/* The "Hello world!" of microcontrollers. Blink LED on/off */
#include <stdint.h>
#include "stm8.h"

#define LED_PORT    PA
#define LED_PIN     PIN3

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

void i2c_inter (void) __interrupt 19 {
	// I2C_SR1;
	// I2C_SR2;
	// I2C_SR3;

	// unsigned char value;

	if(I2C_SR1 & 0b00000010){
		// set_led(1);
		address_received_flag = !address_received_flag;

		// clear ADDR by reading SR1 followed by SR3
		I2C_SR1;
		I2C_SR3;

		return;
	}

	if(I2C_SR1 & 0b01000000){
		value = I2C_DR;

		return;
	}

	if(I2C_SR1 & 0b00010000){

		// clear STOPF by reading SR1 then writing CR2
		I2C_SR1;
		// I2C_CR2 |= 0b00000010;
		I2C_CR2 &= 0b11111101;
		return;
	}

	// I2C_SR1 &= 0b00100000;
	// I2C_SR2 &= 0b11010000;

	return;

	// // set_led(1);
	// // disableInterrupts();

	// // set_led(1);

	// // I2C_SR2 &= 0b11010000;

	// // if(I2C_SR3 & 0b00000000){
	// // 	set_led(1);
	// // 	// I2C_SR1 &= 0b00100000;
	// // 	// I2C_SR2 &= 0b11010000;

 // //        delay(300000L);
	// // }

	// // if(I2C_SR2 & 0b00000001){
	// // 	set_led(1);
	// // 	// I2C_SR2 &= 0b11010000;
	// // }

	// set_led(0);

	// enableInterrupts();

	// 	I2C_SR2 &= 0b11111110;
	// }
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

    value = 10;

	while(1) {

        // // value = analog_read();

		// address_received_flag = !address_received_flag;


        // long delay_length = 300000L;
        if(address_received_flag == 1){
        	set_led(0);
        } else {
        	set_led(1);
        }

        delay(value * 3000L);

        // wfi();
    }
}