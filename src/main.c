/* The "Hello world!" of microcontrollers. Blink LED on/off */
#include <stdint.h>

#include "stm8.h"

#include "i2c.h"
#include "gpio.h"
#include "adc.h"
#include "pwm.h"

#include "utils.h"
#include "data_collector.h"

int address_received_flag;
int i2c_activity;

unsigned char address;

unsigned char i2c_received_value;
// unsigned char temp_value;

int duty = 0;

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
	pwm_init();

	set_led(1);
	pwm_set_duty(0);

	// ADC CHANNELS:
	// 6 -- DIFF Amp output
	// 5 -- Divided Cell_minus
	// 2 -- Balance Current
	// 3 -- Balance Feedback (Voltage)
	// 4 -- Cell Temp

	while(1) {

		unsigned int balance_current = adc_read(2);
		unsigned int balance_feedback = adc_read(3);
		
		int16_t user_reg = i2c_registers[0x22];
		data_to_transmit[0] = lsb(user_reg);

		// Cell Voltage
		float cell_voltage_raw = adc_voltage_on_channel(6);
		float cell_voltage = calibrate_cell_voltage(cell_voltage_raw);
		uint16_t cell_voltage_mv = (uint16_t)(cell_voltage * 1000); 
		
		data_to_transmit[1] = lsb(cell_voltage_mv);
		data_to_transmit[2] = msb(cell_voltage_mv);	

		// Temperature
		float temp_voltage = adc_voltage_on_channel(4);
		float temp = calibrate_temp(temp_voltage);
		uint16_t temp_16bit = (uint16_t)(temp * 10);

		data_to_transmit[3] = lsb(temp_16bit);
		data_to_transmit[4] = msb(temp_16bit);

		// Cell Position (Absolute Cell Minus Voltage)
		unsigned int cell_minus = adc_read(5);
		data_to_transmit[5] = lsb(cell_minus);
		data_to_transmit[6] = msb(cell_minus);

		// LED behavior
		int16_t led_status = i2c_registers[0x23];
		set_led(led_status);

		// Balance Behavior
		uint16_t balance_active = i2c_registers[0x11];
		uint16_t duty_cycle = i2c_registers[0x12];
		uint16_t frequency = i2c_registers[0x13];

		float freq = (float)(frequency);

		if(balance_active) {
			pwm_set_freq(freq);
			pwm_set_duty(duty_cycle);
		} else {
			pwm_set_duty(0);
		}
	}
}