
/*
 * Timer.h
 *
 * Created: 03.10.2026 13:49:09
 *  Author: andrksel
 */ 

#pragma once
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include "../Utils/PinStructure.h"


void timer3_init(void);
void JoyBtnPress(void);


//TCCR1B og TCCR3B - 16 BIT TIMER
//TCCR1A og TCCR3A - 16 BIT TIMER
//TCCR0 - 8 BIT TIMER
//TCCR2 - 8 BIT TIMER


typedef struct Timers{

    volatile uint8_t *TCCR3B_JoystickBtnDeboucing; 
    volatile uint8_t *TCCR1B_free;
    volatile uint8_t *TCCR3A_free;
    volatile uint8_t *TCCR1A_free;

    volatile uint8_t *TCCR0_free;
    volatile uint8_t *TCCR2_free;
    volatile uint16_t *OCR3A_TimerCompareValue;
    volatile uint8_t *ETIMSK_InterruptMask; //ETIMSK 
    
    volatile uint8_t *GIFR_GeneralInterruptFlag;
    volatile uint8_t *GICR_GeneralInterruptControl;

    volatile uint8_t *TCNT3_Timer3Counter;


}TimerStruct_t;

extern TimerStruct_t Timers;
