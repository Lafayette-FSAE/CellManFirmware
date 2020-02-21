#ifndef __DATA_COLLECTOR_H
#define __DATA_COLLECTOR_H

#include "adc.h"

float adc_value_to_voltage(int adc_reading){
	float adc_float = (float) adc_reading;
	return (adc_float * 5) / 1024;
}

float adc_voltage_on_channel(int channel){
	uint16_t adc_value = adc_read(channel);
	float voltage = adc_value_to_voltage(adc_value);

	return voltage;
}

float calibrate_temp(float voltage){
	return voltage * 30.14 - 16.6;
}

float calibrate_cell_voltage(float adc_voltage){
	return (adc_voltage * 2) - 1;
}

#endif // DATA_COLLECTOR_H