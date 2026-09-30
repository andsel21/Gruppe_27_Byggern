/*
 * ADC.h
 *
 * Created: 22.09.2026 15:27:06
 *  Author: andrksel
 */ 


#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <avr/io.h>


void Init_ADC(void);
uint8_t ADC_read_channel(uint8_t channel);
//void ADC_Print(uint8_t channel, uint8_t value);
void ADC_Print(int16_t value0, int16_t value1, int16_t value2,int16_t value3 );
int16_t mapValue(int16_t value, int16_t fromMinValue, int16_t fromMaxValue, int16_t toMinValue, int16_t toMaxValue);