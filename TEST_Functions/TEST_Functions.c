/*
 * TEST_Functions.c
 *
 * Created: 17.09.2026 14:03:25
 *  Author: andrksel
 */ 

#include "TEST_Functions.h"
#include <avr/io.h>
#include <stdio.h>


uint8_t EnableSW = 1;

void sqaureWaveFuncPB0(){
	if (EnableSW == 1) {
		PORTB |= (1 << PB0);   // sett PB0 high
		_delay_ms(250);   // styrer frekvense
		PORTB &= ~(1 << PB0); // sett PB0 low
		_delay_ms(50);  
	}
	else {
	PORTB &= ~(1 << PB0); // sett PB0 low
	}
}

