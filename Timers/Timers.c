
/*
 * Timers.c
 *
 * Created: 03.10.2026 13:48:55
 *  Author: andrksel
 */ 

// F_CPU = 4.9152 MHz
//Målet er å ha 100Hz




#include "Timer.h"



void timer1_init(void){
	//Set Mode 4: CTC with TOP = OCR1A
	TCCR3B	|= (1<<WGM32);
	//Set compare match for 100 Hz (10ms)
	//OCR1A = 7679;
	OCR3A = 767;
}

ISR(TIMER3_COMPA_vect){
	//Timer External interrupt Disable
	ETIMSK &= ~(1<<OCIE3A);
	
	// Clear pending INT0 flag caused by bouncing
	GIFR |= (1 << INTF0);
	//Button External interrupt Enable
	GICR |= (1<<INT0);
	 
	//Prescalar 64: Start Timer (CS11 = 1m CS10 = 1)
	TCCR3B &= ~(1<<CS31);
	TCCR3B &= ~(1 << CS30);
}