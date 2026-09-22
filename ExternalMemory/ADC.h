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