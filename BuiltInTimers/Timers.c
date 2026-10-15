
/*
 * Timers.c
 *
 * Created: 03.10.2026 13:48:55
 *  Author: andrksel
 */ 

// F_CPU = 4.9152 MHz
//M�let er � ha 100Hz


#include "Timers.h"
TimerStruct_t Timers = {
	.TCCR3B_JoystickBtnDeboucing = &TCCR3B,
    .TCCR1B_free = &TCCR1B,
    .TCCR3A_free = &TCCR3A,
    .TCCR1A_free = &TCCR1A,
	.TCCR0_free = &TCCR0,
    .TCCR2_free = &TCCR2,

	.OCR3A_TimerCompareValue = &OCR3A,
	.ETIMSK_InterruptMask = &ETIMSK
};




void timer3_init(void){
	//--------------------------------------------------------
		//Set Mode 4: CTC with TOP = OCR1A
		*Timers.TCCR3B_JoystickBtnDeboucing |= (1<<WGM32);
		//Set compare match for 100 Hz (10ms)
		*Timers.OCR3A_TimerCompareValue = 767; // 10 ms
	//--------------------------------------------------------
}

ISR(TIMER3_COMPA_vect){

	// =========================================================
	//with Timer Struct
	
    static uint8_t releaseSamplesCOPY = 0;

	//PD2 = JoyButton
    if (*PINS.D_Status & (1 << PD2_JoyBtn)) {
        // Still pressed: keep INT0 disabled.
        releaseSamplesCOPY = 0;
        return;
    }

    if (++releaseSamplesCOPY < 3) {
        return;
    }

    releaseSamplesCOPY = 0;

    // Stop Timer3 and disable its compare interrupt.
	*Timers.TCCR3B_JoystickBtnDeboucing &= ~((1 << CS32_Prescaler) | (1 << CS31_Prescaler) | (1 << CS30_Prescaler));
	*Timers.ETIMSK_InterruptMask &= ~(1<<OCIE3A_EnableInterrupt);

    // Clear bounce interrupts before accepting another press.
	*Timers.GIFR_GeneralInterruptFlag = (1 << INTF0_ExternalInterruptFlag0);
	*Timers.GICR_GeneralInterruptControl |= (1 << INT0_ExternalInterrupt0);

   //=========================================================
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
// 	GICR |= (1<<INT0);
	 
// 	//Prescalar 64: Start Timer (CS11 = 1m CS10 = 1)
// 	TCCR3B &= ~(1<<CS31);
// 	TCCR3B &= ~(1 << CS30);
// }