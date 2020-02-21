#ifndef __DATA_COLLECTOR_H
#define __DATA_COLLECTOR_H

#include "adc.h"

float adc_voltage(int adc_reading){
	
	float adc_float = (float) adc_reading;
	return (adc_float * 5) / 1024;
}


float calibrate_temp(float voltage){
	return voltage * 30.14 - 16.6;
}


#endif // DATA_COLLECTOR_H