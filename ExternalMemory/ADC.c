/*
 * ADC.c
 *
 * Created: 22.09.2026 15:26:59
 *  Author: andrksel
 */ 


#include "ADC.h"

#define ADC_BASE ((volatile uint8_t *)0x1000)

/* MAX156 configuration bits */
#define ADC_ALL  (1u << 7)
#define ADC_DIFF (1u << 6)
#define ADC_BIP  (1u << 5)
#define ADC_INH  (1u << 4)
#define ADC_PD   (1u << 3)

/* BUSY connected to PE2 */
#define ADC_BUSY  PE2

void Init_ADC(void)
{
	// PD5 / OC1A as output
	DDRD |= (1 << PD5);

	// Toggle OC1A on compare match
	// Timer1 CTC mode
	TCCR1A = (1 << COM1A0);
	TCCR1B = (1 << WGM12) | (1 << CS10);

	// TOP / compare value
	OCR1A = 1;
		
	// BUSY as input
	DDRE &= ~(1 << ADC_BUSY);
}

uint8_t ADC_read_channel(uint8_t channel)
{
    uint8_t config;

    channel &= 0x03;       // MAX156: channel 0-3

    config = ADC_ALL | channel;

    /*
     * Memory write:
     * ATmega automatically generates /CS + /WR
     * through external memory interface/address decoder.
     */
    *ADC_BASE = config;

    /*
     * BUSY = 0 while converting.
     */
    while (!(PINE & (1 << ADC_BUSY)))
    {
    }

    /*
     * Memory read:
     * ATmega automatically generates /CS + /RD.
     */
    return *ADC_BASE;
}

