/* The "Hello world!" of microcontrollers. Blink LED on/off */
#include <stdint.h>

#include "stm8.h"

#include "i2c.h"
#include "gpio.h"
#include "adc.h"
#include "utils.h"

int address_received_flag;
int i2c_activity;

unsigned char address;

unsigned char value;
// unsigned char temp_value;

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
		// value = I2C_DR;

		return;
	}

	// Check TXE bit
	if(I2C_SR1 & 0b10000000){

		I2C_DR = data_to_transmit[data_index];

		data_index = (data_index + 1) % (data_size + 1);
		// I2C_DR = temp_value;

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


	// configure gpio inputs
	gpio_init_as_input(PORTA, 0);
	gpio_init_as_input(PORTA, 1);
	gpio_init_as_input(PORTC, 3);
	gpio_init_as_input(PORTC, 5);
	gpio_init_as_input(PORTC, 6);
	gpio_init_as_input(PORTC, 7);

	gpio_set_pull_up(PORTA, 0, 1);
	gpio_set_pull_up(PORTA, 1, 1);
	gpio_set_pull_up(PORTC, 3, 1);
	gpio_set_pull_up(PORTC, 4, 1);
	gpio_set_pull_up(PORTC, 6, 1);
	gpio_set_pull_up(PORTC, 7, 1);

	gpio_init_as_output(PORTA, 3);

	unsigned int a0 = gpio_read_from(PORTA, 1);
	unsigned int a1 = gpio_read_from(PORTC, 7);
	unsigned int a2 = gpio_read_from(PORTC, 6);
	unsigned int a3 = gpio_read_from(PORTC, 5);
	unsigned int a4 = gpio_read_from(PORTC, 3);

	int address_bits[] = {a0, a1, a2, a3, a4, 0, 0, 0};
	address = concat_bits(address_bits);


    /* Set clock to full speed (16 Mhz) */
    CLK_CKDIVR = 0;

    /* GPIO setup */
    // Set pin data direction as output
    PORT(LED_PORT, DDR)  |= LED_PIN; // i.e. PB_DDR |= (1 << 5);
    // Set pin as "Push-pull"
	PORT(LED_PORT, CR1)  |= LED_PIN; // i.e. PB_CR1 |= (1 << 5);

	i2c_init(address + 7);
	adc_init();

	// temp_value = 100;

	while(1) {


		unsigned int temp_value = adc_read(4);
		// unsigned int temp_value = 1023;


		unsigned int cell_minus = adc_read(5);
		// unsigned int cell_minus = 1023;

		data_to_transmit[0] = temp_value & 0b11111111;
		data_to_transmit[1] = (temp_value >> 8) & 0b00000011;		

		data_to_transmit[2] = cell_minus & 0b11111111;
		data_to_transmit[3] = (cell_minus >> 8) & 0b00000011;

		// delay(value * 3000L);
	}
}