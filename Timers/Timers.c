
/*
 * Timers.c
 *
 * Created: 03.10.2026 13:48:55
 *  Author: andrksel
 */ 

// F_CPU = 4.9152 MHz
//M�let er � ha 100Hz




#include "Timers.h"




void timer3_init(void){
	//Set Mode 4: CTC with TOP = OCR1A
	TCCR3B	|= (1<<WGM32);
	//Set compare match for 100 Hz (10ms)
	//OCR1A = 7679;
	OCR3A = 767;
}

ISR(TIMER3_COMPA_vect){
	//Gamle kode før debounce bug
	// //Timer External interrupt Disable
	// ETIMSK &= ~(1<<OCIE3A);
	
	// // Clear pending INT0 flag caused by bouncing
	// GIFR |= (1 << INTF0);
	// //Button External interrupt Enable
	// GICR |= (1<<INT0);
	 
	// //Prescalar 64: Start Timer (CS11 = 1m CS10 = 1)
	// TCCR3B &= ~(1<<CS31);
	// TCCR3B &= ~(1 << CS30);

    static uint8_t releaseSamples = 0;

	//PD2 = JoyButton
    if (PIND & (1 << PD2)) {
        // Still pressed: keep INT0 disabled.
        releaseSamples = 0;
        return;
    }

    if (++releaseSamples < 3) {
        return;
    }

    releaseSamples = 0;

    // Stop Timer3 and disable its compare interrupt.
    TCCR3B &= ~((1 << CS32) | (1 << CS31) | (1 << CS30));
    ETIMSK &= ~(1 << OCIE3A);

    // Clear bounce interrupts before accepting another press.
    GIFR = (1 << INTF0);
    GICR |= (1 << INT0);
}



// void timer1_init(void){
// 	//Set Mode 4: CTC with TOP = OCR1A
// 	TCCR3B	|= (1<<WGM32);
// 	//Set compare match for 100 Hz (10ms)
// 	//OCR1A = 7679;
// 	OCR3A = 767;
// }

// ISR(TIMER3_COMPA_vect){
// 	//Timer External interrupt Disable
// 	ETIMSK &= ~(1<<OCIE3A);
	
// 	// Clear pending INT0 flag caused by bouncing
// 	GIFR |= (1 << INTF0);
// 	//Button External interrupt Enable
// 	GICR |= (1<<INT0);    </Compile>

// 	//Prescalar 64: Start Timer (CS11 = 1m CS10 = 1)
// 	TCCR3B &= ~(1<<CS31);
// 	TCCR3B &= ~(1 << CS30);
// }