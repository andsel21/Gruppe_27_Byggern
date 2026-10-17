/*
 * TEST_Functions.c
 *
 * Created: 17.09.2026 14:03:25
 *  Author: andrksel
 */ 

#include "TEST_Functions.h"

uint8_t EnableSW = 1;

void sqaureWaveFuncPB0(){
	if (EnableSW == 1){
	if(CHECK_BIT(DDRB,DDB0) == 0){
		DDRB |= (1u<<DDB0);
	}
	PORTB |= (1 << PB0);   // sett PB0 hih
	_delay_ms(250);   // styrer frekvensen
	PORTB ^= (1 << PB0);  // sett PB0 low
	_delay_ms(250);   // styrer frekvensen
	}
}

