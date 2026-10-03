


#pragma once
#include <avr/io.h>


int16_t mapValue(int16_t value, int16_t fromMinValue, int16_t fromMaxValue, int16_t toMinValue, int16_t toMaxValue)
{
	if(value < fromMinValue){
		value = fromMinValue;
	}
	if(value > fromMaxValue){
		value = fromMaxValue;
	}
	
	double normalizedValue = (double)(value - fromMinValue) / (fromMaxValue - fromMinValue);

	int16_t mappedValue = (int16_t)(normalizedValue * (toMaxValue - toMinValue) + toMinValue);

	if (mappedValue > 100){
		mappedValue = 100;
	}
	if(mappedValue < -100){
		mappedValue = -100;
	}
	return mappedValue;
}