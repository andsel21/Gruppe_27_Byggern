/*
 * ADC.c
 *
 * Created: 22.09.2026 15:26:59
 *  Author: andrksel
 */ 


#include "ADC.h"


#define ADC_TIMEOUT 10000UL

#define ADC_BASE ((volatile uint8_t *)0x1000)

/* MAX156 configuration bits */
#define ADC_ALL  (1u << 7)

/* BUSY connected to PE0 */
#define ADC_BUSY  PE0

void Init_ADC(void)
{
	// PD5 / OC1A as output - connected to CLK (pin9) MAX156
	DDRD |= (1 << PD5);

	DDRD &= ~(1 << PD3);
	PORTD|= (1 << PD3);
	//cli(); ?????????
	// Toggle OC1A on compare match
	// Timer1 CTC mode
	TCCR1A = (1 << COM1A0);
	TCCR1B = (1 << WGM12) | (1 << CS10);

	// TOP / compare value
	OCR1A = 1;
		
	// BUSY as input
	DDRE &= ~(1 << ADC_BUSY);
	//sei(); ?????????????
}


uint8_t ADC_read_channel(uint8_t channel)
{
    uint8_t config;
	uint32_t timeout;
	uint8_t value;

    channel &= 0x03;       // MAX156: channel 0-3

    config = ADC_ALL | channel;

    *ADC_BASE = config;
	
    /*
     * BUSY = 0 while converting.
     */
	
	timeout = ADC_TIMEOUT;
	
	// Vent f�rst p� at BUSY faktisk g�r LOW
	while (PINE & (1 << ADC_BUSY))
	{
		 if (--timeout == 0)
        {
            return 0;
        }
	}
	
	timeout = ADC_TIMEOUT;

	while (!(PINE & (1 << ADC_BUSY))) //Vent mens BUSY e low. 
	{
		if (--timeout == 0)
        {
            return 0;
        }
	}
    /*
     * Memory read:
     * ATmega automatically generates /CS + /RD.
     */
	
	value = *ADC_BASE;
    return value;
}




